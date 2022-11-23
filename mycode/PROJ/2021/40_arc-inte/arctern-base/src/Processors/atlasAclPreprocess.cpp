//
// Created by zy on 8/28/20.
//
#include "atlasAclPreprocess.h"

using namespace arctern;
static auto g_resizeConfigDeleter = [](acldvppResizeConfig *p) { acldvppDestroyResizeConfig(p); };
static auto g_picDescDeleter = [](acldvppPicDesc *picDesc) { acldvppDestroyPicDesc(picDesc); };
static auto g_roiConfigDeleter = [](acldvppRoiConfig *p) { acldvppDestroyRoiConfig(p); };
static auto g_jpegeConfigDeleter = [](acldvppJpegeConfig *p) { acldvppDestroyJpegeConfig(p); };

AtlasAclPreprocess::AtlasAclPreprocess(aclrtContext context, aclrtStream stream):context_(context), dvppStream_(stream){}

AtlasAclPreprocess::~AtlasAclPreprocess(){
  AtlasAclPreprocess::Destroy_DvppMemory();
}

void AtlasAclPreprocess::Destroy_DvppMemory(){
  auto ret = aclrtSetCurrentContext(context_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
  }
  RELEASE_DVPP_DATA(dvppOutputMemPtr);

  if(cropImage_){
    RELEASE_DVPP_DATA(cropImage_->data);
  }

  if(encodedImage_){
    RELEASE_DVPP_DATA(encodedImage_->data);
  }
  acldvppDestroyChannel(dvppChannelDesc_);
  acldvppDestroyChannelDesc(dvppChannelDesc_);
  dvppChannelDesc_ = nullptr;
}
/*
 * @description: Create a channel for processing image data,
 *               the channel description is created by acldvppCreateChannelDesc
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::init(const int &batchsize, const int &resizeWidth, const int &resizeHeight) {
  /// set acl context
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  /// set dvpp channel desc
  dvppChannelDesc_ = acldvppCreateChannelDesc();
  if (dvppChannelDesc_ == nullptr) {
    return -1;
  }
  ret = acldvppCreateChannel(dvppChannelDesc_);
  if (ret != APP_ERR_OK) {
    LOGASDFatal << "Failed to create dvpp channel: " << GetAppErrCodeInfo(ret) << ".";
    acldvppDestroyChannelDesc(dvppChannelDesc_);
    dvppChannelDesc_ = nullptr;
    return ret;
  }

  /// pre-allocate dvpp output memory for resize
  // Get output buffer size for resize output
  maxBatchSize = batchsize;
  ret = GetVpcDataSize(resizeWidth, resizeHeight, PIXEL_FORMAT_YUV_SEMIPLANAR_420, dvppOutputMemSize);
  if (ret != APP_ERR_OK) {
    acldvppDestroyChannel(dvppChannelDesc_);
    acldvppDestroyChannelDesc(dvppChannelDesc_);
    dvppChannelDesc_ = nullptr;
    return ret;
  }
  ret = acldvppMalloc(&dvppOutputMemPtr, (dvppOutputMemSize * maxBatchSize));
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to malloc " << (dvppOutputMemSize * maxBatchSize) << " bytes on dvpp for resize, ret = " << ret << ".";
    return ret;
  }

  return APP_ERR_OK;
}

/*
 * @description: Get the size of buffer used to save image for VPC according to width, height and format
 * @param  width specifies the width of the output image
 * @param  height specifies the height of the output image
 * @param  format specifies the format of the output image
 * @param: vpcSize is used to save the result size
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::GetVpcDataSize(uint32_t width, uint32_t height, acldvppPixelFormat format,
                                             uint32_t &vpcSize) {
  /// Check the invalid format of VPC function and calculate the output buffer size
  if (format != PIXEL_FORMAT_YUV_SEMIPLANAR_420 && format != PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
    LOGASDError << "Format[" << format << "] for VPC is not supported, just support NV12 or NV21.";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  uint32_t widthStride = DVPP_ALIGN_UP(width, VPC_WIDTH_ALIGN);
  uint32_t heightStride = DVPP_ALIGN_UP(height, VPC_HEIGHT_ALIGN);
  vpcSize = widthStride * heightStride * YUV_BGR_SIZE_CONVERT_3 / YUV_BGR_SIZE_CONVERT_2;
  return APP_ERR_OK;
}

/*
 * @description: Get the aligned width and height of the input image according to the image format
 * @param: width specifies the width before alignment
 * @param: height specifies the height before alignment
 * @param: format specifies the image format
 * @param: widthStride is used to save the width after alignment
 * @param: heightStride is used to save the height after alignment
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::GetVpcInputStrideSize(uint32_t width, uint32_t height, acldvppPixelFormat format,
                                                    uint32_t &widthStride, uint32_t &heightStride)
{
  uint32_t inputWidthStride;
  // Check the invalidty of input format and calculate the input width stride
  if (format >= PIXEL_FORMAT_YUV_400 && format <= PIXEL_FORMAT_YVU_SEMIPLANAR_444) {
    // If format is YUV SP, keep widthStride not change.
    inputWidthStride = DVPP_ALIGN_UP(width, VPC_STRIDE_WIDTH);
  } else if (format >= PIXEL_FORMAT_YUYV_PACKED_422 && format <= PIXEL_FORMAT_VYUY_PACKED_422) {
    // If format is YUV422 packed, image size = H x W * 2;
    inputWidthStride = DVPP_ALIGN_UP(width, VPC_STRIDE_WIDTH) * YUV422_WIDTH_NU;
  } else if (format >= PIXEL_FORMAT_YUV_PACKED_444 && format <= PIXEL_FORMAT_BGR_888) {
    // If format is YUV444 packed or RGB, image size = H x W * 3;
    inputWidthStride = DVPP_ALIGN_UP(width, VPC_STRIDE_WIDTH) * YUV444_RGB_WIDTH_NU;
  } else if (format >= PIXEL_FORMAT_ARGB_8888 && format <= PIXEL_FORMAT_BGRA_8888) {
    // If format is XRGB8888, image size = H x W * 4
    inputWidthStride = DVPP_ALIGN_UP(width, VPC_STRIDE_WIDTH) * XRGB_WIDTH_NU;
  } else {
    LOGASDError << "Input format[" << format << "] for VPC is invalid, please check it.";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  uint32_t inputHeightStride = DVPP_ALIGN_UP(height, VPC_STRIDE_HEIGHT);
  // Check the input validity width stride.
  if (inputWidthStride > MAX_RESIZE_WIDTH || inputWidthStride < MIN_RESIZE_WIDTH) {
    LOGASDError << "Input width stride " << inputWidthStride << " is invalid, not in [" << MIN_RESIZE_WIDTH \
                 << ", " << MAX_RESIZE_WIDTH << "].";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  // Check the input validity height stride.
  if (inputHeightStride > MAX_RESIZE_HEIGHT || inputHeightStride < MIN_RESIZE_HEIGHT) {
    LOGASDError << "Input height stride " << inputHeightStride << " is invalid, not in [" << MIN_RESIZE_HEIGHT \
                 << ", " << MAX_RESIZE_HEIGHT << "].";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  widthStride = inputWidthStride;
  heightStride = inputHeightStride;
  return APP_ERR_OK;
}

/*
 * @description: Get the aligned width and height of the output image according to the image format
 * @param: width specifies the width before alignment
 * @param: height specifies the height before alignment
 * @param: format specifies the image format
 * @param: widthStride is used to save the width after alignment
 * @param: heightStride is used to save the height after alignment
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::GetVpcOutputStrideSize(uint32_t width, uint32_t height, acldvppPixelFormat format,
                                                     uint32_t &widthStride, uint32_t &heightStride)
{
  // Check the invalidty of output format and calculate the output width and height
  if (format != PIXEL_FORMAT_YUV_SEMIPLANAR_420 && format != PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
    LOGASDError << "Output format[" << format << "] for VPC is not supported, just support NV12 or NV21.";
    return APP_ERR_COMM_INVALID_PARAM;
  }

  widthStride = DVPP_ALIGN_UP(width, VPC_STRIDE_WIDTH);
  heightStride = DVPP_ALIGN_UP(height, VPC_STRIDE_HEIGHT);
  return APP_ERR_OK;
}

/*
 * @description: Set picture description information and execute resize function
 * @param: input specifies the input image information
 * @param: output specifies the output image information
 * @param: withSynchronize specifies whether to execute synchronously
 * @param: processType specifies whether to perform proportional scaling, not by default
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::VpcResize(DvppDataInfo &input, DvppDataInfo &output, bool withSynchronize,
                                        VpcProcessType processType)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  acldvppPicDesc *inputDesc = acldvppCreatePicDesc();
  acldvppPicDesc *outputDesc = acldvppCreatePicDesc();
  resizeInputDesc_.reset(inputDesc, g_picDescDeleter);
  resizeOutputDesc_.reset(outputDesc, g_picDescDeleter);

  /// Set dvpp picture descriptin info of input image
  ret = SetDvppPicDescData(input, *resizeInputDesc_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set dvpp input picture description, ret = " << ret << ".";
    return ret;
  }

  /// Set dvpp picture descriptin info of output image
  ret = SetDvppPicDescData(output, *resizeOutputDesc_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set dvpp output picture description, ret = " << ret << ".";
    return ret;
  }

  if (processType == VPC_PT_DEFAULT) {
    return ResizeProcess(*resizeInputDesc_, *resizeOutputDesc_, withSynchronize);
  }

  /// Resize with padding, the crop roi area is the whole input image
  /// The left and up offset must be even, right and bottom offset must be odd which is required by acl
  CropRoiConfig cropRoi = {0};
  cropRoi.right = CONVERT_TO_ODD(input.width - ODD_NUM_1);
  cropRoi.down = CONVERT_TO_ODD(input.height - ODD_NUM_1);
  /// The width and height of the original image will be resized by the same ratio
  /// The resized image will be pasted to the upper left corner
  CropRoiConfig pasteRoi = {0};
  GetPasteRoi(input, output, pasteRoi.right, pasteRoi.down);
  return ResizeWithPadding(*resizeInputDesc_, *resizeOutputDesc_, cropRoi, pasteRoi, withSynchronize);
}

APP_ERROR AtlasAclPreprocess::VpcCropResize(DvppCropInputInfo &inputInfo, DvppDataInfo &output, bool withSynchronize,
                                            VpcProcessType processType)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  auto &input = inputInfo.dataInfo;
  acldvppPicDesc *inputDesc = acldvppCreatePicDesc();
  acldvppPicDesc *outputDesc = acldvppCreatePicDesc();
  resizeInputDesc_.reset(inputDesc, g_picDescDeleter);
  resizeOutputDesc_.reset(outputDesc, g_picDescDeleter);

  /// Set dvpp picture descriptin info of input image
  ret = SetDvppPicDescData(input, *resizeInputDesc_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set dvpp input picture description, ret = " << ret << ".";
    return ret;
  }

  /// Set dvpp picture descriptin info of output image
  ret = SetDvppPicDescData(output, *resizeOutputDesc_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set dvpp output picture description, ret = " << ret << ".";
    return ret;
  }
  if (processType == VPC_PT_DEFAULT) {
    return ResizeProcess(*resizeInputDesc_, *resizeOutputDesc_, withSynchronize);
  }

  CropRoiConfig cropRoi = inputInfo.roi;
  /// The width and height of the original image will be resized by the same ratio
  /// The resized image will be pasted to the upper left corner
  CropRoiConfig pasteRoi = {0};
  if(processType == VPC_PT_PADDING){
    GetPasteRoi(input, output, pasteRoi.right, pasteRoi.down);
  }else if(processType == VPC_PT_BOTTOM){
    pasteRoi.right = CONVERT_TO_ODD(output.width);
    pasteRoi.down = CONVERT_TO_ODD(output.height);
  }
  return ResizeWithPadding(*resizeInputDesc_, *resizeOutputDesc_, cropRoi, pasteRoi, withSynchronize);
}

/*
 * @description: Set image description information
 * @param: dataInfo specifies the image information
 * @param: picsDesc specifies the picture description information to be set
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::SetDvppPicDescData(const DvppDataInfo &dataInfo, acldvppPicDesc &picDesc)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  ret = acldvppSetPicDescData(&picDesc, dataInfo.data);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set data for dvpp picture description, ret = " << ret << ".";
    return ret;
  }
  ret = acldvppSetPicDescSize(&picDesc, dataInfo.dataSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set size for dvpp picture description, ret = " << ret << ".";
    return ret;
  }
  ret = acldvppSetPicDescFormat(&picDesc, dataInfo.format);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set format for dvpp picture description, ret = " << ret << ".";
    return ret;
  }
  ret = acldvppSetPicDescWidth(&picDesc, dataInfo.width);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set width for dvpp picture description, ret = " << ret << ".";
    return ret;
  }
  ret = acldvppSetPicDescHeight(&picDesc, dataInfo.height);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set height for dvpp picture description, ret = " << ret << ".";
    return ret;
  }
  ret = acldvppSetPicDescWidthStride(&picDesc, dataInfo.widthStride);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set aligned width for dvpp picture description, ret = " << ret << ".";
    return ret;
  }
  ret = acldvppSetPicDescHeightStride(&picDesc, dataInfo.heightStride);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set aligned height for dvpp picture description, ret = " << ret << ".";
    return ret;
  }

  return APP_ERR_OK;
}

/*
 * @description: Check whether the image format and zoom ratio meet the requirements
 * @param: input specifies the input image information
 * @param: output specifies the output image information
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::CheckResizeParams(const DvppDataInfo &input, const DvppDataInfo &output)
{
  if (output.format != PIXEL_FORMAT_YUV_SEMIPLANAR_420 && output.format != PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
    LOGASDError << "Output format[" << output.format << "] for VPC is not supported, just support NV12 or NV21.";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  if (((float)output.height / input.height) < MIN_RESIZE_SCALE ||
      ((float)output.height / input.height) > MAX_RESIZE_SCALE) {
    LOGASDError << "Resize scale should be in range [1/16, 32], which is " << (output.height / input.height) << ".";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  if (((float)output.width / input.width) < MIN_RESIZE_SCALE ||
      ((float)output.width / input.width) > MAX_RESIZE_SCALE) {
    LOGASDError << "Resize scale should be in range [1/16, 32], which is " << (output.width / input.width) << ".";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  return APP_ERR_OK;
}

/*
 * @description: Get paste area
 * @param: input specifies the input image information
 * @param: output specifies the output image information
 * @param: pasteRightOffset is used to save the width of the paste area
 * @param: pasteBottomOffset is used to save the height of the paste area
 * @return: APP_ERR_OK if success, other values if failure
 */
void AtlasAclPreprocess::GetPasteRoi(const DvppDataInfo &input, const DvppDataInfo &output, uint32_t &pasteRightOffset,
                                     uint32_t &pasteBottomOffset)
{
  float resizeRatio = static_cast<float>(input.width) / output.width;
  if (resizeRatio < (static_cast<float>(input.height) / output.height)) {
    resizeRatio = static_cast<float>(input.height) / output.height;
  }

  pasteRightOffset = static_cast<float>(input.width) / resizeRatio;
  if ((pasteRightOffset % MODULUS_NUM_2) == 0) {
    pasteRightOffset = pasteRightOffset - 1; // must be odd
  }

  pasteBottomOffset = static_cast<float>(input.height) / resizeRatio;
  if ((pasteBottomOffset % MODULUS_NUM_2) == 0) {
    pasteBottomOffset = pasteBottomOffset - 1; // must be odd
  }
}

/*
 * @description: Scale the input image to the size specified by the output image and
 *               saves the result to the output image (non-proportionate scaling)
 * @param: inputDesc specifies the description information of the input image
 * @param: outputDesc specifies the description information of the output image
 * @param: withSynchronize specifies whether to execute synchronously
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::ResizeProcess(acldvppPicDesc &inputDesc, acldvppPicDesc &outputDesc, bool withSynchronize)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  acldvppResizeConfig *resizeConfig = acldvppCreateResizeConfig();
  if (resizeConfig == nullptr) {
    LOGASDError << "Failed to create dvpp resize config.";
    return APP_ERR_COMM_INVALID_POINTER;
  }

  resizeConfig_.reset(resizeConfig, g_resizeConfigDeleter);
  ret = acldvppVpcResizeAsync(dvppChannelDesc_, &inputDesc, &outputDesc, resizeConfig_.get(), dvppStream_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to resize asynchronously, ret = " << ret << ".";
    return ret;
  }

  if (withSynchronize) {
    ret = aclrtSynchronizeStream(dvppStream_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "Failed to synchronize stream, ret = " << ret << ".";
      return ret;
    }
  }
  return APP_ERR_OK;
}

/*
 * @description: Crop the image from the input image based on the specified area and
 *               paste the cropped image to the specified position of the target image
 *               as the output image
 * @param: inputDesc specifies the description information of the input image
 * @param: outputDesc specifies the description information of the output image
 * @param: cropRoi specifies the cropped area
 * @param: pasteRoi specifies the pasting area
 * @param: withSynchronize specifies whether to execute synchronously
 * @return: APP_ERR_OK if success, other values if failure
 * @attention: If the width and height of the crop area are different from those of the
 *             paste area, the image is scaled again
 */
APP_ERROR AtlasAclPreprocess::ResizeWithPadding(acldvppPicDesc &inputDesc, acldvppPicDesc &outputDesc, CropRoiConfig &cropRoi,
                                                CropRoiConfig &pasteRoi, bool withSynchronize)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  acldvppRoiConfig *cropRoiCfg = acldvppCreateRoiConfig(cropRoi.left, cropRoi.right, cropRoi.up, cropRoi.down);
  if (cropRoiCfg == nullptr) {
    LOGASDError << "Failed to create dvpp roi config for corp area.";
    return APP_ERR_COMM_FAILURE;
  }
  cropAreaConfig_.reset(cropRoiCfg, g_roiConfigDeleter);

  acldvppRoiConfig *pastRoiCfg = acldvppCreateRoiConfig(CONVERT_TO_EVEN(pasteRoi.left), CONVERT_TO_ODD(pasteRoi.right),
                                                        CONVERT_TO_EVEN(pasteRoi.up), CONVERT_TO_ODD(pasteRoi.down));
  if (pastRoiCfg == nullptr) {
    LOGASDError << "Failed to create dvpp roi config for paster area.";
    return APP_ERR_COMM_FAILURE;
  }
  pasteAreaConfig_.reset(pastRoiCfg, g_roiConfigDeleter);

  ret = acldvppVpcCropAndPasteAsync(dvppChannelDesc_, &inputDesc, &outputDesc, cropAreaConfig_.get(),
                                              pasteAreaConfig_.get(), dvppStream_);
  if (ret != APP_ERR_OK) {
    // release resource.
    LOGASDError << "Failed to crop and paste asynchronously, ret = " << ret << ".";
    return ret;
  }
  if (withSynchronize) {
    ret = aclrtSynchronizeStream(dvppStream_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "Failed tp synchronize stream, ret = " << ret << ".";
      return ret;
    }
  }
  return APP_ERR_OK;
}

/*
 * @description: Resize the image specified by input and save the result to member variable resizedImage_
 * @param: input specifies the input image information
 * @param: output specifies the output image information
 * @param: withSynchronize specifies whether to execute synchronously
 * @param: processType specifies whether to perform proportional scaling, not by default
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::CombineResizeProcess(DvppDataInfo &input, DvppDataInfo &output, bool withSynchronize,
                                                   VpcProcessType processType, size_t offsetAddr)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  ret = CheckResizeParams(input, output);
  if (ret != APP_ERR_OK) {
    return ret;
  }

  resizedImage_ = std::make_shared<DvppDataInfo>();
  resizedImage_->width = output.width;
  resizedImage_->height = output.height;
  resizedImage_->format = output.format;
  ret = GetVpcOutputStrideSize(output.width, output.height, output.format, resizedImage_->widthStride,
                               resizedImage_->heightStride);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  // Get output buffer size for resize output
  ret = GetVpcDataSize(output.width, output.height, output.format, resizedImage_->dataSize);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  // check the size of pre-allocate is equal to the resizedImage_->dataSize
  if(resizedImage_->dataSize == dvppOutputMemSize){
    resizedImage_->data = (uint8_t*)dvppOutputMemPtr + offsetAddr;
  }else{
    LOGASDError << "Error: the resizedImage_->dataSize = " << resizedImage_->dataSize << " isn't equal to the dvppOutputMemSize = " << dvppOutputMemSize;
    return APP_ERR_COMM_FAILURE;
  }
  resizedImage_->frameId = input.frameId;
  ret = VpcResize(input, *resizedImage_, withSynchronize, processType);
  if (ret != APP_ERR_OK) {
    return ret;
  }

  return ret;
}

APP_ERROR AtlasAclPreprocess::CombineResizeBatchProcess(std::vector<DvppDataInfo> &inputInfos, std::vector<DvppDataInfo> &outputInfos, bool withSynchronize,
                                                        VpcProcessType processType){
  currentBatchSize = inputInfos.size();
  if(currentBatchSize > maxBatchSize){
    LOGASDError << "Error: the batchsize of input = " << currentBatchSize << " is more than the maxBatchsize = " << maxBatchSize;
    return APP_ERR_COMM_FAILURE;
  }

  size_t offsetAddr = 0;
  for(int i = 0; i < currentBatchSize; ++i){
    auto ret = CombineResizeProcess(inputInfos[i], outputInfos[i], withSynchronize, processType, offsetAddr);
    if(ret != APP_ERR_OK){
      LOGASDError << "error:  CombineResizeProcess of i = " << i << " is failed";
      return APP_ERR_COMM_FAILURE;
    }
    offsetAddr += dvppOutputMemSize;
  }

  return APP_ERR_OK;
}

APP_ERROR AtlasAclPreprocess::CombineCropResizeProcess(DvppCropInputInfo &inputInfo, DvppDataInfo &output, bool withSynchronize,
                                                       VpcProcessType processType, size_t offsetAddr){
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  /// check crop params
  /// The left and up offset must be even, right and bottom offset must be odd which is required by acl
  auto &cropRoi = inputInfo.roi;
  cropRoi.left = CONVERT_TO_EVEN(cropRoi.left);
  cropRoi.up = CONVERT_TO_EVEN(cropRoi.up);
  cropRoi.right = CONVERT_TO_ODD(cropRoi.right);
  cropRoi.down = CONVERT_TO_ODD(cropRoi.down);
  ret = CheckCropParams(inputInfo);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  /// check resize params
  auto& input = inputInfo.dataInfo;
  resizedImage_ = std::make_shared<DvppDataInfo>();
  resizedImage_->width = output.width;
  resizedImage_->height = output.height;
  resizedImage_->format = output.format;
  ret = GetVpcOutputStrideSize(output.width, output.height, output.format, resizedImage_->widthStride,
                               resizedImage_->heightStride);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  // Get output buffer size for resize output
  ret = GetVpcDataSize(output.width, output.height, output.format, resizedImage_->dataSize);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  // check the size of pre-allocate is equal to the resizedImage_->dataSize
  if(resizedImage_->dataSize == dvppOutputMemSize){
    resizedImage_->data = (uint8_t*)dvppOutputMemPtr + offsetAddr;
  }else{
    LOGASDError << "error: the resizedImage_->dataSize = " << resizedImage_->dataSize << " isn't equal to the dvppOutputMemSize = " << dvppOutputMemSize;
    return APP_ERR_COMM_FAILURE;
  }
  resizedImage_->frameId = input.frameId;
  ret = VpcCropResize(inputInfo, *resizedImage_, withSynchronize, processType);
  if (ret != APP_ERR_OK) {
    return ret;
  }

  return ret;
}

APP_ERROR AtlasAclPreprocess::CombineCropResizeBatchProcess(std::vector<DvppCropInputInfo> &inputInfos, std::vector<DvppDataInfo> &outputInfos,
                                                            bool withSynchronize, VpcProcessType processType){
  currentBatchSize = inputInfos.size();
  
  if(currentBatchSize > maxBatchSize){
    LOGASDError << "Error: the batchsize of input = " << currentBatchSize << " is more than the maxBatchsize = " << maxBatchSize;
    return APP_ERR_COMM_FAILURE;
  }

  size_t offsetAddr = 0;
  for(int i = 0; i < currentBatchSize; ++i){
    auto ret = CombineCropResizeProcess(inputInfos[i], outputInfos[i], withSynchronize, processType, offsetAddr);
    if(ret != APP_ERR_OK){
      LOGASDError << "error:  CombineCropResizeProcess of i = " << i << " is failed";
      return APP_ERR_COMM_FAILURE;
    }
    offsetAddr += dvppOutputMemSize;
  }

  return APP_ERR_OK;
}
/*
 * @description: Set picture description information and execute crop function
 * @param: cropInput specifies the input image information and cropping area
 * @param: output specifies the output image information
 * @param: withSynchronize specifies whether to execute synchronously
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::VpcCrop(const DvppCropInputInfo &cropInput, const DvppDataInfo &output, bool withSynchronize)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  acldvppPicDesc *inputDesc = acldvppCreatePicDesc();
  acldvppPicDesc *outputDesc = acldvppCreatePicDesc();
  cropInputDesc_.reset(inputDesc, g_picDescDeleter);
  cropOutputDesc_.reset(outputDesc, g_picDescDeleter);

  // Set dvpp picture descriptin info of input image
  ret = SetDvppPicDescData(cropInput.dataInfo, *cropInputDesc_);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  // Set dvpp picture descriptin info of output image
  ret = SetDvppPicDescData(output, *cropOutputDesc_);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  return CropProcess(*cropInputDesc_, *cropOutputDesc_, cropInput.roi, withSynchronize);
}

/*
 * @description: Check whether the size of the cropped data and the cropped area meet the requirements
 * @param: input specifies the image information and the information about the area to be cropped
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::CheckCropParams(DvppCropInputInfo &input)
{
  APP_ERROR ret;
  uint32_t payloadSize;
  ret = GetVpcDataSize(input.dataInfo.widthStride, input.dataInfo.heightStride, PIXEL_FORMAT_YUV_SEMIPLANAR_420,
                       payloadSize);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  if (payloadSize != input.dataInfo.dataSize) {
    LOGASDError << "Input data size: " << payloadSize << " to crop does not match input yuv image size: " \
                 << input.dataInfo.dataSize << ".";
    return APP_ERR_COMM_INVALID_PARAM;
  }

  if ((!CHECK_EVEN(input.roi.left)) || (!CHECK_EVEN(input.roi.up)) || (!CHECK_ODD(input.roi.right)) ||
      (!CHECK_ODD(input.roi.down))) {
    LOGASDError << "Crop area left and top(" << input.roi.left << ", " << input.roi.up \
                 << ") must be even, right bottom(" << input.roi.right << "," << input.roi.down << ") must be odd.";
    return APP_ERR_COMM_INVALID_PARAM;
  }

  // Calculate crop width and height according to the input location
  uint32_t cropWidth = input.roi.right - input.roi.left + ODD_NUM_1;
  uint32_t cropHeight = input.roi.down - input.roi.up + ODD_NUM_1;
  if ((cropWidth < MIN_CROP_WIDTH) || (cropHeight < MIN_CROP_HEIGHT)) {
    LOGASDError << "Crop area width:" << cropWidth << " need to be larger than 10 and height:" << cropHeight
             << " need to be larger than 6.";
    return APP_ERR_COMM_INVALID_PARAM;
  }

  if ((input.roi.left + cropWidth >= input.dataInfo.width) || (input.roi.up + cropHeight >= input.dataInfo.height)) {
//    LOGASDError << "Target rectangle start location(" << input.roi.left << "," << input.roi.up << ") with size("
//             << cropWidth << "," << cropHeight << ") is out of the input image(" << input.dataInfo.width << ","
//             << input.dataInfo.height << ") to be cropped.";
//    return APP_ERR_COMM_INVALID_PARAM;
    /// fixed: compatiblity handle
    if(input.roi.left + cropWidth >= input.dataInfo.width){
      uint32_t diff = (input.roi.left + cropWidth) - input.dataInfo.width;
      input.roi.right  = CONVERT_TO_ODD((input.roi.right - diff - 1));
    }
    if(input.roi.up + cropHeight >= input.dataInfo.height){
      uint32_t diff = (input.roi.up + cropHeight) - input.dataInfo.height;
      input.roi.down = CONVERT_TO_ODD((input.roi.down - diff - 1));
    }
  }

  return APP_ERR_OK;
}


/*
 * @description: It is used to crop an input image based on a specified region and
 *               store the cropped image to the output memory as an output image
 * @param: inputDesc specifies the description information of the input image
 * @param: outputDesc specifies the description information of the output image
 * @param: CropRoiConfig specifies the cropped area
 * @param: withSynchronize specifies whether to execute synchronously
 * @return: APP_ERR_OK if success, other values if failure
 * @attention: if the region of the output image is inconsistent with the crop area, the image is scaled again
 */
APP_ERROR AtlasAclPreprocess::CropProcess(acldvppPicDesc &inputDesc, acldvppPicDesc &outputDesc,
                                          const CropRoiConfig &cropArea, bool withSynchronize)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  uint32_t leftOffset = CONVERT_TO_EVEN(cropArea.left);
  uint32_t rightOffset = CONVERT_TO_ODD(cropArea.right);
  uint32_t upOffset =  CONVERT_TO_EVEN(cropArea.up);
  uint32_t downOffset = CONVERT_TO_ODD(cropArea.down);

  auto cropRioCfg = acldvppCreateRoiConfig(leftOffset, rightOffset, upOffset, downOffset);
  if (cropRioCfg == nullptr) {
    LOGASDError << "DvppCommon: create dvpp vpc resize failed.";
    return APP_ERR_DVPP_RESIZE_FAIL;
  }
  cropRoiConfig_.reset(cropRioCfg, g_roiConfigDeleter);

  ret = acldvppVpcCropAsync(dvppChannelDesc_, &inputDesc, &outputDesc, cropRoiConfig_.get(), dvppStream_);
  if (ret != APP_ERR_OK) {
    // release resource.
    LOGASDError << "Failed to crop, ret = " << ret << ".";
    return ret;
  }
  if (withSynchronize) {
    ret = aclrtSynchronizeStream(dvppStream_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "Failed to synchronize stream, ret = " << ret << ".";
      return ret;
    }
  }
  return APP_ERR_OK;
}

/*
 * @description: Crop the image specified by the input parameter and saves the result to member variable cropImage_
 * @param: input specifies the input image information and cropping area
 * @param: output specifies the output image information
 * @param: withSynchronize specifies whether to execute synchronously
 * @return: APP_ERR_OK if success, other values if failure
 */
APP_ERROR AtlasAclPreprocess::CombineCropProcess(DvppCropInputInfo &input, DvppDataInfo &output, bool withSynchronize)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  if (cropImage_ != nullptr) { /// clear pre-called
    RELEASE_DVPP_DATA(cropImage_->data);
  }
  // Get widthStride and heightStride for input and output image according to the format
  ret = GetVpcInputStrideSize(input.dataInfo.width, input.dataInfo.height, input.dataInfo.format,
                              input.dataInfo.widthStride, input.dataInfo.heightStride);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  ret = CheckCropParams(input);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  cropImage_ = std::make_shared<DvppDataInfo>();
  cropImage_->width = output.width;
  cropImage_->height = output.height;
  cropImage_->format = output.format;
  ret = GetVpcOutputStrideSize(output.width, output.height, output.format, cropImage_->widthStride,
                               cropImage_->heightStride);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  // Get output buffer size for resize output
  ret = GetVpcDataSize(output.width, output.height, output.format, cropImage_->dataSize);
  if (ret != APP_ERR_OK) {
    return ret;
  }

  // Malloc buffer for output of resize module
  // Need to pay attention to release of the buffer
  ret = acldvppMalloc((void **)(&(cropImage_->data)), cropImage_->dataSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to malloc " << cropImage_->dataSize << " bytes on dvpp for resize, ret = " << ret << ".";
    return ret;
  }
  cropImage_->frameId = input.dataInfo.frameId;
  ret = VpcCrop(input, *cropImage_, withSynchronize);
  if (ret != APP_ERR_OK) {
    // Release the output buffer when resize failed, otherwise release it after use
    RELEASE_DVPP_DATA(cropImage_->data);
  }
  return ret;
}

APP_ERROR AtlasAclPreprocess::GetJpegEncodeStrideSize(std::shared_ptr<DvppDataInfo> &inputImage)
{
  uint32_t inputWidth = inputImage->width;
  uint32_t inputHeight = inputImage->height;
  acldvppPixelFormat format = inputImage->format;
  uint32_t widthStride;
  uint32_t heightStride;
  uint32_t encodedBufferSize;
  // Align up the input width and height and calculate buffer size of encoded input file
  if (format == PIXEL_FORMAT_YUV_SEMIPLANAR_420 || format == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
    widthStride = DVPP_ALIGN_UP(inputWidth, JPEGE_STRIDE_WIDTH);
    heightStride = DVPP_ALIGN_UP(inputHeight, JPEGE_STRIDE_HEIGHT);
    encodedBufferSize = widthStride * heightStride * YUV_BYTES_NU / YUV_BYTES_DE;
  }else if (format == PIXEL_FORMAT_YUYV_PACKED_422 || format == PIXEL_FORMAT_UYVY_PACKED_422 ||
            format == PIXEL_FORMAT_YVYU_PACKED_422 || format == PIXEL_FORMAT_VYUY_PACKED_422) {
    widthStride = DVPP_ALIGN_UP(inputWidth * YUV422_WIDTH_NU, JPEGE_STRIDE_WIDTH);
    heightStride = DVPP_ALIGN_UP(inputHeight, JPEGE_STRIDE_HEIGHT);
    encodedBufferSize = widthStride * heightStride;
  }else {
    return APP_ERR_COMM_INVALID_PARAM;
  }
  if (encodedBufferSize == 0) {
    LOGASDError << "Input host buffer size is empty.";
    return APP_ERR_COMM_INVALID_PARAM;
  }
  inputImage->widthStride = widthStride;
  inputImage->heightStride = heightStride;
  inputImage->dataSize = encodedBufferSize;
  return APP_ERR_OK;
}

APP_ERROR AtlasAclPreprocess::SetEncodeLevel(uint32_t level, acldvppJpegeConfig& jpegeConfig)
{
  // Set the encoding quality
  // The coding quality range [0, 100]
  // The level 0 coding quality is similar to the level 100
  // The smaller the value in [1, 100], the worse the quality of the output picture
  auto ret = (APP_ERROR)acldvppSetJpegeConfigLevel(&jpegeConfig, level);
  if (ret != APP_ERR_OK) {
    return ret;
  }
  return APP_ERR_OK;
}

APP_ERROR AtlasAclPreprocess::GetJpegEncodeDataSize(DvppDataInfo &input, acldvppJpegeConfig *jpegeConfig, uint32_t &encSize)
{
  acldvppPicDesc *inputDesc = acldvppCreatePicDesc();
  encodeInputDesc_.reset(inputDesc, g_picDescDeleter);

  APP_ERROR  ret = SetDvppPicDescData(input, *encodeInputDesc_);
  if (ret != APP_ERR_OK) {
    return ret;
  }

  uint32_t outputSize;
  ret = acldvppJpegPredictEncSize(encodeInputDesc_.get(), jpegeConfig, &outputSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to predict encode size of jpeg image, ret = " << ret << ".";
    return ret;
  }
  encSize = outputSize;
  return APP_ERR_OK;
}

APP_ERROR AtlasAclPreprocess::JpegEncode(DvppDataInfo &input, DvppDataInfo &output, acldvppJpegeConfig *jpegeConfig,
                                         bool withSynchronize)
{
  APP_ERROR  ret = SetDvppPicDescData(input, *encodeInputDesc_);
  if (ret != APP_ERR_OK) {
    return ret;
  }

  ret = acldvppJpegEncodeAsync(dvppChannelDesc_, encodeInputDesc_.get(), output.data, &output.dataSize, jpegeConfig, dvppStream_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to encode image, ret = " << ret << ".";
    return ret;
  }
  if (withSynchronize) {
    ret = aclrtSynchronizeStream(dvppStream_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "Failed to aclrtSynchronizeStream, ret = " << ret << ".";
      return APP_ERR_DVPP_JPEG_ENCODE_FAIL;
    }
  }
  LOGASDInfo << "Encode successfully." ;
  return APP_ERR_OK;
}

APP_ERROR AtlasAclPreprocess::CombineJpegeProcess(std::shared_ptr<DvppDataInfo> &input, bool withSynchronize)
{
  SWITCH_ATLAS_ACL_CONTEXT(context_);

  auto jpegeConfig = acldvppCreateJpegeConfig();
  jpegeConfig_.reset(jpegeConfig, g_jpegeConfigDeleter);

  uint32_t encodeLevel = 100;
  ret = SetEncodeLevel(encodeLevel, *jpegeConfig_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set encode level, ret = " << ret << ".";
    return ret;
  }

  // Get the buffer size of encode output according to the input data and jpeg encode config
  uint32_t encodeOutBufferSize;
  ret = GetJpegEncodeDataSize(*input, jpegeConfig_.get(), encodeOutBufferSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to get size of encode output buffer, ret = " << ret << ".";
    return ret;
  }

  encodedImage_ = std::make_shared<DvppDataInfo>();
  encodedImage_->dataSize = encodeOutBufferSize;
  // Malloc dvpp buffer to store the output data after decoding
  // Need to pay attention to release of the buffer
  ret = acldvppMalloc((void **)&encodedImage_->data, encodedImage_->dataSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to malloc memory on dvpp, ret = " << ret << ".";
    return ret;
  }

  // Encode input image
  ret = JpegEncode(*input, *encodedImage_, jpegeConfig_.get(), withSynchronize);
  if (ret != APP_ERR_OK) {
    // Release the output buffer when decode failed, otherwise release it after use
    acldvppFree(encodedImage_->data);
    return ret;
  }
  return APP_ERR_OK;
}

APP_ERROR AtlasAclPreprocess::WriteResult(const std::string& fileName, const std::shared_ptr<void>& outBuf,
                                          const uint32_t& encodeOutDataSize)
{

  std::string outputFileName = fileName + ".jpg";
  FILE *outFileFp = fopen(outputFileName.c_str(), "wb+"); // Open the output file
  if (nullptr == outFileFp) {
    LOGASDError << "Failed to open file " << outputFileName << ".";
    return APP_ERR_COMM_OPEN_FAIL;
  }
  size_t writeSize = fwrite(outBuf.get(), 1, encodeOutDataSize, outFileFp); // Write the output file
  fflush(outFileFp);
  fclose(outFileFp);
  return APP_ERR_OK;
}

APP_ERROR AtlasAclPreprocess::WriteEncodeImgToFile(const std::string &filePath, std::shared_ptr<DvppDataInfo> &encodeOutData){
  SWITCH_ATLAS_ACL_CONTEXT(context_);
  void* resHostBuf = nullptr;
  // Malloc host memory to save the output data
  ret = (APP_ERROR)aclrtMallocHost(&resHostBuf, encodeOutData->dataSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to malloc memory on host, ret = " << ret << ".";
    return ret;
  }
  std::shared_ptr<void> outBuf(resHostBuf, aclrtFreeHost);
  ret = (APP_ERROR)aclrtMemcpy(outBuf.get(), encodeOutData->dataSize, encodeOutData->data,
                               encodeOutData->dataSize, ACL_MEMCPY_DEVICE_TO_HOST); // Copy the output data from device to host
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to copy memory from device to host, ret = " << ret << ".";
    return ret;
  }
  ret = WriteResult(filePath, outBuf, encodeOutData->dataSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to write result file, ret = " << ret << ".";
    return ret;
  }

  return APP_ERR_OK;
}

std::shared_ptr<DvppDataInfo> AtlasAclPreprocess::GetResizedImage()
{
  return resizedImage_;
}


std::shared_ptr<DvppDataInfo> AtlasAclPreprocess::GetCropedImage()
{
  return cropImage_;
}

std::shared_ptr<DvppDataInfo> AtlasAclPreprocess::GetEncodedImage()
{
  return encodedImage_;
}

std::pair<void*, size_t> AtlasAclPreprocess::GetPreprocessedDvppPtr(){
  return std::make_pair(dvppOutputMemPtr, (dvppOutputMemSize * maxBatchSize));
}


///=======================================================================================================



DvppDecodeManager::DvppDecodeManager(int deviceId){

  auto ret = aclrtCreateContext(&context_, deviceId);
  if (ret != 0) {
    LOGASDError << "Failed to create acl context, ret = " << ret << "." ;
    return; 
  }

  LOGASDInfo  << "Created context for device " << deviceId << " successfully" ;

  aclrtSetCurrentContext(context_);
  if(ret != 0){
    LOGASDError << "Failed to set ACL context, ret = " << ret ;
    exit(-1);
  }

  ret = aclrtCreateStream(&dvppStream_);
  if (ret != 0) {
    LOGASDError << "Failed to create the acl stream, ret = " << ret << "." ;
    exit(-1);
  }

  dvppChannelDesc_ = acldvppCreateChannelDesc();
  if (dvppChannelDesc_ == nullptr) {
    LOGASDError << "error: dvppChannelDesc_ is nullptr!" ;
    exit(-1);
  }

  ret = acldvppCreateChannel(dvppChannelDesc_);
  if (ret != 0) {
    LOGASDError << "Failed to create dvpp channel: " ;
    acldvppDestroyChannelDesc(dvppChannelDesc_);
    dvppChannelDesc_ = nullptr;
    exit(-1);
  }
}

void DvppDecodeManager::Init(){
  
}

DvppDecodeManager::~DvppDecodeManager() {
  destroy();
  auto ret = aclrtDestroyContext(context_);
  if (ret != 0) {
    LOGASDError << "Failed to destroy context, ret = " << ret << "." ;
  }
}

void DvppDecodeManager::destroy() {
  auto ret = aclrtSetCurrentContext(context_);
  if(ret != 0){
    LOGASDError << "Failed to set ACL context, ret = " << ret ;
    exit(-1);
  }

  ret = aclrtDestroyStream(dvppStream_);
  if(ret != 0) {
    LOGASDError << "Failed to destroy the stream, ret = " << ret << "." ;
    exit(-1);
  }

  ret = acldvppDestroyChannel(dvppChannelDesc_);
  if (ret != 0) {
    LOGASDError << "Failed to destory dvpp channel, ret = " << ret << "." ;
    exit(-1);
  }

  ret = acldvppDestroyChannelDesc(dvppChannelDesc_);
  if (ret != 0) {
    LOGASDError << "Failed to destroy dvpp channel description, ret = " << ret << "." ;
    exit(-1);
  }

  /// free dvpp memory
  for(auto& e: v_decodedImage_){
    RELEASE_DVPP_DATA(e->data);
    e->data = nullptr;
  }
}

bool DvppDecodeManager::GetJpegImageInfo(const void *data, uint32_t dataSize, 
                                         uint32_t &width, uint32_t &height,
                                         int32_t &components)
{
  uint32_t widthTmp;
  uint32_t heightTmp;
  int32_t componentsTmp;
  auto ret = acldvppJpegGetImageInfo(data, dataSize, &widthTmp, &heightTmp, &componentsTmp);
  if (ret != 0) {
    LOGASDError << "Failed to get image info of jpeg, ret = " << ret << "." ;
    return false;
  }
  if (widthTmp > MAX_JPEGD_WIDTH || widthTmp < MIN_JPEGD_WIDTH) {
    LOGASDError << "Input width is invalid, not in [" << MIN_JPEGD_WIDTH << ", " << MAX_JPEGD_WIDTH << "]." ;
    return false;
  }
  if (heightTmp > MAX_JPEGD_HEIGHT || heightTmp < MIN_JPEGD_HEIGHT) {
    LOGASDError << "Input height is invalid, not in [" << MIN_JPEGD_HEIGHT << ", " << MAX_JPEGD_HEIGHT << "]." ;
    return false;
  }
  width = widthTmp;
  height = heightTmp;
  components = componentsTmp;
  return true;
}

bool DvppDecodeManager::GetJpegDecodeDataSize(const void *data, uint32_t dataSize, acldvppPixelFormat format, uint32_t &decSize)
{
  uint32_t outputSize;
  auto ret = acldvppJpegPredictDecSize(data, dataSize, format, &outputSize);
  if (ret != 0) {
    LOGASDError << "Failed to predict decode size of jpeg image, ret = " << ret << "." ;
    return false;
  }
  decSize = outputSize;
  return true;
}

bool DvppDecodeManager::TransferImageH2D(const ArcternAtlasImgData& imageInfo, const std::shared_ptr<DvppDataInfo>& jpegInput)
{
  // Check image buffer size validity
  if (imageInfo.rawDataLen <= 0) {
    LOGASDError << "The input buffer size on host should not be empty." ;
    return false;
  }

  uint8_t* inDevBuff = nullptr;
  auto ret = acldvppMalloc((void **)&inDevBuff, imageInfo.rawDataLen);
  if (ret != 0) {
    LOGASDError << "Failed to malloc " << imageInfo.rawDataLen << " bytes on dvpp, ret = " << ret << "." ;
    return false;
  }

  // Copy the image data from host to device
  ret = aclrtMemcpyAsync(inDevBuff, imageInfo.rawDataLen, imageInfo.rawData, imageInfo.rawDataLen,
                         ACL_MEMCPY_HOST_TO_DEVICE, dvppStream_);
  if (ret != 0) {
    LOGASDError << "Failed to copy " << imageInfo.rawDataLen << " bytes from host to device, ret = " << ret << "." ;
    RELEASE_DVPP_DATA(inDevBuff);
    return false;
  }
  // Attention: We must call the aclrtSynchronizeStream to ensure the task of memory replication has been completed
  // after calling aclrtMemcpyAsync
  ret = aclrtSynchronizeStream(dvppStream_);
  if (ret != 0) {
    LOGASDError << "Failed to synchronize stream, ret = " << ret << "." ;
    RELEASE_DVPP_DATA(inDevBuff);
    return false;
  }
  jpegInput->data = inDevBuff;
  jpegInput->dataSize = imageInfo.rawDataLen;

  return true;
}

void DvppDecodeManager::GetJpegDecodeStrideSize(uint32_t width, uint32_t height, uint32_t &widthStride, uint32_t &heightStride)
{
  widthStride = DVPP_ALIGN_UP(width, JPEGD_STRIDE_WIDTH);
  heightStride = DVPP_ALIGN_UP(height, JPEGD_STRIDE_HEIGHT);
}

bool DvppDecodeManager::SetDvppPicDescData(const DvppDataInfo &dataInfo, acldvppPicDesc &picDesc)
{
  auto ret = acldvppSetPicDescData(&picDesc, dataInfo.data);
  if (ret != 0) {
    LOGASDError << "Failed to set data for dvpp picture description, ret = " << ret << "." ;
    return false;
  }
  ret = acldvppSetPicDescSize(&picDesc, dataInfo.dataSize);
  if (ret != 0) {
    LOGASDError << "Failed to set size for dvpp picture description, ret = " << ret << "." ;
    return false;
  }
  ret = acldvppSetPicDescFormat(&picDesc, dataInfo.format);
  if (ret != 0) {
    LOGASDError << "Failed to set format for dvpp picture description, ret = " << ret << "." ;
    return false;
  }
  ret = acldvppSetPicDescWidth(&picDesc, dataInfo.width);
  if (ret != 0) {
    LOGASDError << "Failed to set width for dvpp picture description, ret = " << ret << "." ;
    return false;
  }
  ret = acldvppSetPicDescHeight(&picDesc, dataInfo.height);
  if (ret != 0) {
    LOGASDError << "Failed to set height for dvpp picture description, ret = " << ret << "." ;
    return false;
  }

  return true;
}

bool DvppDecodeManager::JpegDecode(DvppDataInfo &input, DvppDataInfo &output, bool withSynchronize)
{
  acldvppPicDesc *outputDesc = acldvppCreatePicDesc();
  decodeOutputDesc_.reset(outputDesc, g_picDescDeleter);

  auto ret = SetDvppPicDescData(output, *decodeOutputDesc_);
  if (!ret) {
    return false;
  }

  ret = acldvppJpegDecodeAsync(dvppChannelDesc_, input.data, input.dataSize, decodeOutputDesc_.get(), dvppStream_);
  if (ret != 0) {
    LOGASDError << "Failed to decode jpeg, ret = " << ret << "." ;
    return false;
  }
  if (withSynchronize) {
    ret = aclrtSynchronizeStream(dvppStream_);
    if (ret != 0) {
      LOGASDError << "Failed to synchronize stream, ret = " << ret << "." ;
      return false;
    }
  }
  LOGASDInfo << "Decode successfully." ;
  return true;
}

bool DvppDecodeManager::CombineJpegdProcess(const ArcternAtlasImgData& imageInfo, acldvppPixelFormat format, bool withSynchronize)
{
  auto ret = aclrtSetCurrentContext(context_);
  if(ret != 0){
    LOGASDError << "Failed to set ACL context, ret = " << ret ;
    exit(-1);
  }

  int32_t components;
  inputImage_ = std::make_shared<DvppDataInfo>();
  inputImage_->format = format;
  ret = GetJpegImageInfo(imageInfo.rawData, imageInfo.rawDataLen, inputImage_->width, inputImage_->height, components);
  if (!ret) {
    LOGASDError << "Failed to get input image info" ;
    return false;
  }

  // Get the buffer size of decode output according to the input data and output format
  uint32_t outBuffSize;
  ret = GetJpegDecodeDataSize(imageInfo.rawData, imageInfo.rawDataLen, format, outBuffSize);
  if (!ret) {
    LOGASDError << "Failed to get size of decode output buffer" ;
    return false;
  }

  // In TransferImageH2D function, device buffer will be alloced to store the input image
  // Need to pay attention to release of the buffer
  ret = TransferImageH2D(imageInfo, inputImage_);
  if (!ret) {
    RELEASE_DVPP_DATA(inputImage_->data);
    return false;
  }

  auto decodedImage = std::make_shared<DvppDataInfo>();
  decodedImage->format = format;
  decodedImage->width = inputImage_->width;
  decodedImage->height = inputImage_->height;
  GetJpegDecodeStrideSize(inputImage_->width, inputImage_->height, decodedImage->widthStride, decodedImage->heightStride);
  decodedImage->dataSize = outBuffSize;
  // Malloc dvpp buffer to store the output data after decoding
  // Need to pay attention to release of the buffer
  ret = acldvppMalloc((void **)&decodedImage->data, decodedImage->dataSize);
  if (ret != 0) {
    LOGASDError << "Failed to malloc memory on dvpp, ret = " << ret << "." ;
    RELEASE_DVPP_DATA(inputImage_->data);
    RELEASE_DVPP_DATA(decodedImage->data);
    return ret;
  }

  ret = JpegDecode(*inputImage_, *decodedImage, withSynchronize);

  RELEASE_DVPP_DATA(inputImage_->data);
  inputImage_->data = nullptr;
  if (!ret) {
    // Release the output buffer when decode failed, otherwise release it after use
    RELEASE_DVPP_DATA(decodedImage->data);
    decodedImage->data = nullptr;
    return false;
  }

  v_decodedImage_.push_back(decodedImage);
  return true;
}

std::shared_ptr<DvppDataInfo> DvppDecodeManager::GetDecodedImage(){
  int size = v_decodedImage_.size();
  if( size > 0){
    return v_decodedImage_[size - 1];
  }else{
    return nullptr;
  }
}

