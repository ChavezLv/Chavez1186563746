//
// Created by zy on 8/28/20.
//

#ifndef ARCTERN_ATLASACLPREPROCESS_H
#define ARCTERN_ATLASACLPREPROCESS_H

#include "acl/ops/acl_dvpp.h"
#include "src/common/atlas/atlasLog.h"
#include "src/common/atlas/errorCode.h"
#include "src/common/atlas/atlasAclDataType.h"
#include "include/arctern.h"
#include <memory>

namespace arctern{

    const int MODULUS_NUM_2 = 2;
    const uint32_t ODD_NUM_1 = 1;
    const uint32_t JPEGD_STRIDE_WIDTH = 128; // Jpegd module output width need to align up to 128
    const uint32_t JPEGD_STRIDE_HEIGHT = 16; // Jpegd module output height need to align up to 16
    const uint32_t JPEGE_STRIDE_WIDTH = 16; // Jpege module input width need to align up to 16
    const uint32_t JPEGE_STRIDE_HEIGHT = 1; // Jpege module input height remains unchanged
    const uint32_t VPC_STRIDE_WIDTH = 16; // Vpc module output width need to align up to 16
    const uint32_t VPC_STRIDE_HEIGHT = 2; // Vpc module output height need to align up to 2
    const uint32_t VDEC_STRIDE_WIDTH = 16; // Vdec module output width need to align up to 16
    const uint32_t VDEC_STRIDE_HEIGHT = 2; // Vdec module output width need to align up to 2
    const uint32_t YUV_BYTES_NU = 3; // Numerator of yuv image, H x W x 3 / 2
    const uint32_t YUV_BYTES_DE = 2; // Denominator of yuv image, H x W x 3 / 2
    const uint32_t YUV422_WIDTH_NU = 2; // Width of YUV422, WidthStride = Width * 2
    const uint32_t YUV444_RGB_WIDTH_NU = 3; // Width of YUV444 and RGB888, WidthStride = Width * 3
    const uint32_t XRGB_WIDTH_NU = 4; // Width of XRGB8888, WidthStride = Width * 4
    const uint32_t JPEG_OFFSET = 8; // Offset of input file for jpegd module
    const uint32_t MAX_JPEGD_WIDTH = 8192; // Max width of jpegd module
    const uint32_t MAX_JPEGD_HEIGHT = 8192; // Max height of jpegd module
    const uint32_t MIN_JPEGD_WIDTH = 32; // Min width of jpegd module
    const uint32_t MIN_JPEGD_HEIGHT = 32; // Min height of jpegd module
    const uint32_t MAX_JPEGE_WIDTH = 8192; // Max width of jpege module
    const uint32_t MAX_JPEGE_HEIGHT = 8192; // Max height of jpege module
    const uint32_t MIN_JPEGE_WIDTH = 32; // Min width of jpege module
    const uint32_t MIN_JPEGE_HEIGHT = 32; // Min height of jpege module
    const uint32_t MAX_RESIZE_WIDTH = 4096; // Max width stride of resize module
    const uint32_t MAX_RESIZE_HEIGHT = 4096; // Max height stride of resize module
    const uint32_t MIN_RESIZE_WIDTH = 32; // Min width stride of resize module
    const uint32_t MIN_RESIZE_HEIGHT = 6; // Min height stride of resize module
    const float MIN_RESIZE_SCALE = 0.03125; // Min resize scale of resize module
    const float MAX_RESIZE_SCALE = 16.0; // Min resize scale of resize module
    const uint32_t MAX_VPC_WIDTH = 4096; // Max width of picture to VPC(resize/crop)
    const uint32_t MAX_VPC_HEIGHT = 4096; // Max height of picture to VPC(resize/crop)
    const uint32_t MIN_VPC_WIDTH  = 32;   // Min width of picture to VPC(resize/crop)
    const uint32_t MIN_VPC_HEIGHT = 6;    // Min height of picture to VPC(resize/crop)
    const uint32_t MIN_CROP_WIDTH = 10;   // Min width of crop area
    const uint32_t MIN_CROP_HEIGHT = 6;    // Min height of crop area

    #define CONVERT_TO_ODD(NUM) (((NUM) % MODULUS_NUM_2 != 0) ? (NUM) : ((NUM) - 1)) // Convert the input to odd num
    #define CONVERT_TO_EVEN(NUM) (((NUM) % MODULUS_NUM_2 == 0) ? (NUM) : ((NUM) - 1)) // Convert the input to even num
    #define CHECK_ODD(num) ((num) % MODULUS_NUM_2 != 0)
    #define CHECK_EVEN(num) ((num) % MODULUS_NUM_2 == 0)
    #define RELEASE_DVPP_DATA(dvppDataPtr) do { \
        APP_ERROR retMacro; \
        if (dvppDataPtr != nullptr) { \
            retMacro = acldvppFree(dvppDataPtr); \
            if (retMacro != APP_ERR_OK) { \
                LOGASDError << "Failed to free memory on dvpp, ret = " << retMacro << "."; \
            } \
            dvppDataPtr = nullptr; \
        } \
    } while (0);


    ///===========================================================================================================///


    class AtlasAclPreprocess{
    public:
        AtlasAclPreprocess(aclrtContext context, aclrtStream stream);
        ~AtlasAclPreprocess();
        void Destroy_DvppMemory();

        APP_ERROR init(const int &batchsize, const int &resizeWidth, const int &resizeHeight);

        static APP_ERROR GetVpcDataSize(uint32_t widthVpc, uint32_t heightVpc, acldvppPixelFormat format,
                                        uint32_t &vpcSize);
        static APP_ERROR GetVpcInputStrideSize(uint32_t width, uint32_t height, acldvppPixelFormat format,
                                               uint32_t &widthStride, uint32_t &heightStride);
        static APP_ERROR GetVpcOutputStrideSize(uint32_t width, uint32_t height, acldvppPixelFormat format,
                                                uint32_t &widthStride, uint32_t &heightStride);

        APP_ERROR VpcResize(DvppDataInfo &input, DvppDataInfo &output, bool withSynchronize,
                            VpcProcessType processType = VPC_PT_DEFAULT);
        APP_ERROR VpcCropResize(DvppCropInputInfo &inputInfo, DvppDataInfo &output, bool withSynchronize,
                                VpcProcessType processType = VPC_PT_DEFAULT);
        APP_ERROR VpcCrop(const DvppCropInputInfo &input, const DvppDataInfo &output, bool withSynchronize);

        APP_ERROR JpegEncode(DvppDataInfo &input, DvppDataInfo &output, acldvppJpegeConfig *jpegeConfig, bool withSynchronize);

        /// These functions started with "Combine" encapsulate the DVPP process together, malloc DVPP memory,
        /// transfer pictures from host to device, and then execute the DVPP operation.
        /// The caller needs to pay attention to the release of the memory alloced in these functions.
        /// You can call the ReleaseDvppBuffer function to release memory after use completely.
        APP_ERROR CombineResizeProcess(DvppDataInfo &input, DvppDataInfo &output, bool withSynchronize,
                                       VpcProcessType processType = VPC_PT_DEFAULT, size_t offsetAddr = 0);
        APP_ERROR CombineResizeBatchProcess(std::vector<DvppDataInfo> &inputInfos, std::vector<DvppDataInfo> &outputInfos, bool withSynchronize,
                                            VpcProcessType processType = VPC_PT_DEFAULT);

        APP_ERROR CombineCropResizeProcess(DvppCropInputInfo &inputInfo, DvppDataInfo &output, bool withSynchronize,
                                           VpcProcessType processType = VPC_PT_BOTTOM, size_t offsetAddr = 0);
        APP_ERROR CombineCropResizeBatchProcess(std::vector<DvppCropInputInfo> &inputInfos, std::vector<DvppDataInfo> &outputInfos, bool withSynchronize,
                                                VpcProcessType processType = VPC_PT_BOTTOM);
        
        APP_ERROR CombineCropProcess(DvppCropInputInfo &input, DvppDataInfo &output, bool withSynchronize);

        APP_ERROR CombineJpegeProcess(std::shared_ptr<DvppDataInfo> &input, bool withSynchronize);

        APP_ERROR WriteEncodeImgToFile(const std::string &filePath, std::shared_ptr<DvppDataInfo> &encodeOutData);

        std::shared_ptr<DvppDataInfo> GetResizedImage();
        std::shared_ptr<DvppDataInfo> GetCropedImage();
        std::shared_ptr<DvppDataInfo> GetEncodedImage();
        std::pair<void*, size_t> GetPreprocessedDvppPtr();

    private:
        APP_ERROR SetDvppPicDescData(const DvppDataInfo &dataInfo, acldvppPicDesc &picDesc);
        APP_ERROR ResizeProcess(acldvppPicDesc &inputDesc, acldvppPicDesc &outputDesc, bool withSynchronize);
        APP_ERROR ResizeWithPadding(acldvppPicDesc &inputDesc, acldvppPicDesc &outputDesc, CropRoiConfig &cropRoi,
                                    CropRoiConfig &pasteRoi, bool withSynchronize);
        void GetPasteRoi(const DvppDataInfo &input, const DvppDataInfo &output, uint32_t &pasteRightOffset,
                         uint32_t &pasteBottomOffset);

        APP_ERROR CropProcess(acldvppPicDesc &inputDesc, acldvppPicDesc &outputDesc, const CropRoiConfig &cropArea,
                              bool withSynchronize);
        APP_ERROR CheckResizeParams(const DvppDataInfo &input, const DvppDataInfo &output);
        APP_ERROR CheckCropParams(DvppCropInputInfo &input);
        APP_ERROR CheckCropRoi(CropRoiConfig & roi, uint32_t &inputWidth, uint32_t &inputHeight);

        APP_ERROR GetJpegEncodeStrideSize(std::shared_ptr<DvppDataInfo> &inputImage);
        APP_ERROR SetEncodeLevel(uint32_t level, acldvppJpegeConfig& jpegeConfig);
        APP_ERROR GetJpegEncodeDataSize(DvppDataInfo &input, acldvppJpegeConfig *jpegeConfig, uint32_t &encSize);

        APP_ERROR WriteResult(const std::string& fileName, const std::shared_ptr<void>& outBuf, const uint32_t& encodeOutDataSize);

    private:

        aclrtContext context_ = nullptr;
        aclrtStream dvppStream_ = nullptr;

        std::shared_ptr<acldvppPicDesc> encodeInputDesc_ = nullptr;
        std::shared_ptr<acldvppJpegeConfig> jpegeConfig_ = nullptr;

        std::shared_ptr<acldvppPicDesc> cropInputDesc_ = nullptr;
        std::shared_ptr<acldvppPicDesc> cropOutputDesc_ = nullptr;
        std::shared_ptr<acldvppRoiConfig> cropRoiConfig_ = nullptr;

        std::shared_ptr<DvppDataInfo> cropImage_ = nullptr;
        std::shared_ptr<DvppDataInfo> encodedImage_ = nullptr;

        ///>------------------------------------------------------------------

        std::shared_ptr<acldvppRoiConfig> cropAreaConfig_ = nullptr;
        std::shared_ptr<acldvppRoiConfig> pasteAreaConfig_ = nullptr;

        std::shared_ptr<acldvppPicDesc> resizeInputDesc_ = nullptr;
        std::shared_ptr<acldvppPicDesc> resizeOutputDesc_ = nullptr;
        std::shared_ptr<acldvppResizeConfig> resizeConfig_ = nullptr;

        acldvppChannelDesc *dvppChannelDesc_ = nullptr;
        std::shared_ptr<DvppDataInfo> resizedImage_ = nullptr;

        void *dvppOutputMemPtr = nullptr;
        uint32_t dvppOutputMemSize;
        int maxBatchSize = 1;
        int currentBatchSize = 1;
    };


    //---------------------------------------------------------------------------------------------------------------

    class DvppDecodeManager{
    public:
        DvppDecodeManager(int deviceId);
        void Init();
        ~DvppDecodeManager();
        void destroy();

        bool JpegDecode(DvppDataInfo &input, DvppDataInfo &output, bool withSynchronize);
       
        bool CombineJpegdProcess(const ArcternAtlasImgData &imageInfo, acldvppPixelFormat format, bool withSynchronize);
        
        std::shared_ptr<DvppDataInfo> GetDecodedImage();
       
        bool GetJpegDecodeDataSize(const void *data, uint32_t dataSize, acldvppPixelFormat format, uint32_t &decSize);
        
        bool GetJpegImageInfo(const void *data, uint32_t dataSize, uint32_t &width, uint32_t &height, int32_t &components);
        
        bool TransferImageH2D(const ArcternAtlasImgData& imageInfo, const std::shared_ptr<DvppDataInfo>& jpegInput);
        
        void GetJpegDecodeStrideSize(uint32_t width, uint32_t height, uint32_t &widthStride, uint32_t &heightStride);
        
        bool SetDvppPicDescData(const DvppDataInfo &dataInfo, acldvppPicDesc &picDesc);
        
    private:
        aclrtContext context_ = nullptr;
        aclrtStream dvppStream_ = nullptr;
        acldvppChannelDesc *dvppChannelDesc_ = nullptr;
       
        std::shared_ptr<DvppDataInfo> inputImage_ = nullptr;
        std::vector<std::shared_ptr<DvppDataInfo>> v_decodedImage_;
        std::shared_ptr<acldvppPicDesc> decodeOutputDesc_ = nullptr;
    
    };
}
#endif //ARCTERN_ATLASACLPREPROCESS_H
