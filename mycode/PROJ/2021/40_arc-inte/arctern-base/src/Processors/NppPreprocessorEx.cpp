//
// Created by Admin on 2020/6/7.
//

#include <spdlog/spdlog.h>
#include "NppPreprocessorEx.h"
#include "ImgPreProcess/ImgPreprocessorOnNppEx.h"
#include "src/common/imageConvert.h"
#define CHECK_CODE() if(m_lastErrorCode != 0) return m_lastErrorCode;
namespace arctern {
//NppPreprocessorInitPt &NppPreprocessorInitPt::operator=(const NppPreprocessorInitPt &initParam) {
//  if (&initParam == this) return *this;
//  return *this;
//}
NppPreprocessorRunPt::NppPreprocessorRunPt() {
  arcternImage = std::make_unique<DeviceBuffer>();
}
NppPreprocessorRunPt::~NppPreprocessorRunPt() noexcept {
}

NppPreprocessorRet::NppPreprocessorRet() {
  arcternImage = std::make_unique<DeviceBuffer>();
}
NppPreprocessorRet::~NppPreprocessorRet() {
}

NppPreprocessor::NppPreprocessor(cudaStream_t *pStream) :
m_initPt(nullptr), m_lastErrorCode(0), m_nppProcEx(pStream) {

}

ErrorType NppPreprocessor::Init(NppPreprocessorInitPt *p) {
  if (nullptr == p) {
    return ErrorType::ERR_PRE_INIT_PARAM;
  }

  m_initPt = std::make_unique<NppPreprocessorInitPt>();
  *m_initPt = *p;

  return ErrorType::ERR_SUCCESS;
}
std::unique_ptr<NppPreprocessorRet> NppPreprocessor::Process( NppPreprocessorRunPt *p) {
  std::unique_ptr<NppPreprocessorRet> result;
  if (nullptr == p || p->arcternImage->data() == nullptr) {
    return result;
  }
  if (!m_initPt) {
    return result;
  }

  for(auto type : p->sequence_) {
    switch (type) {
      case OperatorType::cvtColor :
        cvtColor(p);
        break;
      case OperatorType::crop:
        crop(p);
        break;
      case OperatorType::affine:
        affine(p);
        break;
      case OperatorType::normalized:
        normalize(p);
        break;
      case OperatorType::extractChannel:
        extractChannel(p);
        break;
      case OperatorType::padding:
        padding(p);
        break;
      case OperatorType::resized:
        resize(p);
        break;
      case OperatorType::detFace:
        detFace(p);
        break;
      case OperatorType::twiceResized:
        twiceResize(p);
        break;
      case OperatorType::multiCat:
        multiCat(p);
        break;
      case OperatorType::splitNormalized:
        splitNormalized(p);
        break;
      case OperatorType::faceJoy:
        faceJoy(p);
        break;
      case OperatorType::merge:
        merge(p);
        break;
      case OperatorType::faceExp:
        faceExp(p);
        break;
      case OperatorType::detFace2:
        detFace2(p);
        break;
      case OperatorType::faceFeat:
        faceFeat(p);
        break;
    }
  }

  result.reset(new NppPreprocessorRet);
  result->arcternImage.swap(p->arcternImage);

  return result;
}

int NppPreprocessor::faceFeat(NppPreprocessorRunPt *pRunParam){
  int model_w = 144;
  int model_h = 160;
  float resize_ratiow = model_w * 1.0 / pRunParam->m_roi.width;
  float resize_ratioh = model_h * 1.0 / pRunParam->m_roi.height;
  int imgw = pRunParam->arcternImage->Width();
  int imgh = pRunParam->arcternImage->Height();
  int new_w = roundf(imgw * resize_ratiow);
  int new_h = roundf(imgh * resize_ratioh);
  ImgPreprocessEx::DeviceBuffer midMat(new_h, new_w,
                                       pRunParam->arcternImage->Channel(),
                                       pRunParam->arcternImage->Type());
  m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage, midMat, ArcternInterpolation ::LINEAR);
  CHECK_CODE()
  int crop_x1 = roundf(pRunParam->m_roi.x * resize_ratiow);
  int crop_y1 = roundf(pRunParam->m_roi.y * resize_ratioh);
  int crop_x2 = roundf((pRunParam->m_roi.x + pRunParam->m_roi.width)*resize_ratiow);
  int crop_y2 = roundf((pRunParam->m_roi.y + pRunParam->m_roi.height)*resize_ratioh);
  int ncrop_x = std::max(-crop_x1, 0);
  int ncrop_y = std::max(-crop_y1, 0);
  crop_x1 = crop_x1 < 0 ? 0 : crop_x1;
  crop_y1 = crop_y1 < 0 ? 0 : crop_y1;
  crop_x2 = crop_x2 < new_w ? crop_x2 : new_w;
  crop_y2 = crop_y2 < new_h ? crop_y2 : new_h;

  crop_x2 = crop_x2-crop_x1+ncrop_x <= model_w ? crop_x2 : crop_x1 + model_w - ncrop_x;
  crop_y2 = crop_y2-crop_y1+ncrop_y <= model_h ? crop_y2 : crop_y1 + model_h - ncrop_y;
  ArcternRect crop_rect;
  crop_rect.x = crop_x1;
  crop_rect.y = crop_y1;
  crop_rect.width = crop_x2 - crop_x1;
  crop_rect.height = crop_y2 - crop_y1;
  ImgPreprocessEx::DeviceBuffer cropMat(crop_rect.height, crop_rect.width,
                                        midMat.Channel(), midMat.Type());
  m_lastErrorCode = m_nppProcEx.Crop(midMat, cropMat, crop_rect);
  CHECK_CODE()
  DeviceBuffer *dstMat = new DeviceBuffer ;
  dstMat->resize(model_h, model_w,
                 cropMat.Channel(),
                 cropMat.Type());
  const int borderColors[]{0, 0, 0, 0};
  m_lastErrorCode = m_nppProcEx.Padding(cropMat, *dstMat, ncrop_y, ncrop_x, borderColors);
  CHECK_CODE()
  pRunParam->arcternImage.reset(dstMat);
  //printDeviceBuffer<uchar>(*pRunParam->arcternImage);
  return m_lastErrorCode;
}

int NppPreprocessor::faceExp(NppPreprocessorRunPt *pRunParam) {
  ArcternRect roi {3,3,90,90};

  auto  * dst = new DeviceBuffer ;
  if (dst->data() == nullptr) {
    dst->resize(roi.height, roi.width,
                pRunParam->arcternImage->Channel(),
                pRunParam->arcternImage->Type());
  }

  m_lastErrorCode = m_nppProcEx.Crop(*pRunParam->arcternImage, *dst, roi);
  m_lastErrorCode = cudaGetLastError();

  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::faceJoy(NppPreprocessorRunPt *pRunParam) {

  auto &lmk = pRunParam->m_lmk;
  auto &image = pRunParam->arcternImage;

  auto rotate_point = [](int ref_x, int ref_y, int x, int y, float angle,
                         int &rotated_x, int &rotated_y) {
    float x0 = x - ref_x;
    float y0 = y - ref_y;
    rotated_x = x0 * std::cos(angle) - y0 * std::sin(angle) + ref_x;
    rotated_y = x0 * std::sin(angle) + y0 * std::cos(angle) + ref_y;
  };

  const float offset_ratio_x = 0.375f;
  const float offset_ratio_y = 0.5f;
  const int exptected_width = 64;
  const int exptected_height = 64;

  const int &left_eye_left_corner_x = lmk[0].x;
  const int &left_eye_left_corner_y = lmk[0].y;
  const int &left_eye_right_corner_x = lmk[1].x;
  const int &left_eye_right_corner_y = lmk[1].y;
  const int &right_eye_left_corner_x = lmk[2].x;
  const int &right_eye_left_corner_y = lmk[2].y;
  const int &right_eye_right_corner_x = lmk[3].x;
  const int &right_eye_right_corner_y = lmk[3].y;
  const int &nose_x = lmk[4].x;
  const int &nose_y = lmk[4].y;
  const int &mouse_left_corner_x = lmk[5].x;
  const int &mouse_left_corner_y = lmk[5].y;
  const int &mouse_right_corner_x = lmk[6].x;
  const int &mouse_right_corner_y = lmk[6].y;

  int left_eye_x = (left_eye_left_corner_x + left_eye_right_corner_x) / 2;
  int left_eye_y = (left_eye_left_corner_y + left_eye_right_corner_y) / 2;
  int right_eye_x = (right_eye_left_corner_x + right_eye_right_corner_x) / 2;
  int right_eye_y = (right_eye_left_corner_y + right_eye_right_corner_y) / 2;
  int eye_x = (left_eye_x + right_eye_x) / 2;
  int eye_y = (left_eye_y + right_eye_y) / 2;
  int mouse_x = (mouse_left_corner_x + mouse_right_corner_x) / 2;
  int mouse_y = (mouse_left_corner_y + mouse_right_corner_y) / 2;

  float reference_height = exptected_height * 0.375f;
  assert(reference_height >= 1);

  float angle = std::atan2(static_cast<float>(right_eye_y - left_eye_y),
                           static_cast<float>(right_eye_x - left_eye_x)) *
      180.0f / M_PI;

  cv::Mat rotation_matrix =
      cv::getRotationMatrix2D(cv::Point(nose_x, nose_y), angle, 1);

  Mat2Array<double> trans;
  double array[2][3] = {0.0};
  trans.Transform<2, 3>(rotation_matrix, array);

  DeviceBuffer affineImg(image->Height(),image->Width(),image->Channel(),nvinfer1::DataType::kINT8);
  ///>npp
  /*nppiWarpAffine_8u_C3R(static_cast<const Npp8u *>(image->data()),
                        {image->Width(),image->Height()},
                        image->Width() * image->Channel() * image->getElementSize(),
                        {0,0,image->Width(),image->Height()},
                        static_cast<Npp8u *>(affineImg.data()),
                        affineImg.Width() * affineImg.Channel() * affineImg.getElementSize(),
                        {0,0,affineImg.Width(),affineImg.Height()}, array,
                        NppiInterpolationMode::NPPI_INTER_LINEAR);*/
  ///> opencv
  cv::Mat srcCvMat(image->Height(),image->Width(),CV_8UC3);
  cudaMemcpy(srcCvMat.data,image->data(),image->nbBytes(),cudaMemcpyDeviceToHost);
  cv::Mat affineCvMat;
  cv::warpAffine(srcCvMat,affineCvMat,rotation_matrix,srcCvMat.size());
  cudaMemcpy(affineImg.data(),affineCvMat.data,affineImg.nbBytes(),cudaMemcpyHostToDevice);
  ///> cuda kernel
  //m_lastErrorCode = m_nppProcEx.Affine(*image,affineImg,array,ArcternInterpolation::LINEAR);
  CHECK_CODE()
  //printDeviceBuffer<uchar>(affineImg);

  angle *= -1.0f / 180.0f * M_PI;

  int rotated_eye_x;
  int rotated_eye_y;
  int rotated_mouth_x;
  int rotated_mouth_y;

  rotate_point(nose_x, nose_y, eye_x, eye_y, angle, rotated_eye_x,
               rotated_eye_y);
  rotate_point(nose_x, nose_y, mouse_x, mouse_y, angle, rotated_mouth_x,
               rotated_mouth_y);

  int affineImg_width = affineImg.Width();
  int affineImg_height = affineImg.Height();

  float scale = reference_height * 1.0f / (rotated_mouth_y - rotated_eye_y);


  ///>cuda
  DeviceBuffer resizeImg;
  m_lastErrorCode = m_nppProcEx.Resize(affineImg,resizeImg,ArcternInterpolation::LINEAR,scale,scale);
  CHECK_CODE()

  ///>opencv
  /*cv::Mat srcMat(affineImg.Height(),affineImg.Width(), CV_MAKETYPE(CV_8U,affineImg.Channel()));
  cudaMemcpy((void*)srcMat.data,affineImg.data(),affineImg.nbBytes(),cudaMemcpyDeviceToHost);
  cv::Mat dstMat;
  cv::resize(srcMat,dstMat,cv::Size(),scale,scale,cv::InterpolationFlags::INTER_LINEAR);
  DeviceBuffer resizeImg(dstMat.rows,dstMat.cols,dstMat.channels(),affineImg.Type());
  cudaMemcpy(resizeImg.data(),(void *)dstMat.data,resizeImg.nbBytes(),cudaMemcpyHostToDevice);*/
  //printDeviceBuffer<uchar>(resizeImg);

  rotated_eye_x = (rotated_eye_x - affineImg_width / 2) * scale +
      resizeImg.Width() / 2;
  rotated_eye_y = (rotated_eye_y - affineImg_height / 2) * scale +
      resizeImg.Height() / 2;

  ArcternRect box{static_cast<float >(rotated_eye_x - exptected_width / 2),
                  static_cast<float >(rotated_eye_y - reference_height),
                  static_cast<float >(exptected_width),
                  static_cast<float >(exptected_height)};

  int left = (box.x < 0) ? (-box.x) : 0;
  int top = (box.y < 0) ? (-box.y) : 0;
  int right = (box.x + box.width > resizeImg.Width())
              ? (box.x + box.width - resizeImg.Width())
              : 0;
  int bottom = (box.y + box.height > resizeImg.Height())
               ? (box.y + box.height - resizeImg.Height())
               : 0;
  box.x += left;
  box.y += top;

  DeviceBuffer paddingImg(resizeImg.Height() + top + bottom,
                     resizeImg.Width() + left + right,
                     resizeImg.Channel(),resizeImg.Type());
  const int borderColors[]{0,0,0,0};
  m_lastErrorCode = m_nppProcEx.Padding(resizeImg,paddingImg,top,left,borderColors);
  CHECK_CODE()
  //printDeviceBuffer<uchar>(paddingImg);

  DeviceBuffer cropImg(box.height,box.width, paddingImg.Channel(),paddingImg.Type());
  m_lastErrorCode = m_nppProcEx.Crop(paddingImg,cropImg,box);
  CHECK_CODE()
  //printDeviceBuffer<uchar>(cropImg);

  ArcternRect roi{2,2,60,60};
  auto *dst = new DeviceBuffer ;
  dst->resize(roi.height,roi.width,cropImg.Channel(),cropImg.Type());
  m_lastErrorCode = m_nppProcEx.Crop(cropImg,*dst,roi);
  CHECK_CODE()

  //printDeviceBuffer<uchar>(*dst);
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::multiCat(NppPreprocessorRunPt *pRunParam) {
  const auto input_w = m_initPt->netW_;
  const auto input_h = m_initPt->netH_;

  const auto img_w = pRunParam->arcternImage->Width();
  const auto img_h = pRunParam->arcternImage->Height();

  int dest_w(img_w);
  int dest_h(img_h);

  // if input_w = input_h
  if (1.0 * input_w / img_w < 1.0 * input_h / img_h) {
    dest_w = input_w;
    dest_h = (img_h * input_w) / img_w;
  } else {
    dest_h = input_h;
    dest_w = (img_w * input_h) / img_h;
  }

  ImgPreprocessEx::DeviceBuffer midMat(dest_h, dest_w,
                                       pRunParam->arcternImage->Channel(),
                                       pRunParam->arcternImage->Type());

  m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage, midMat,\
                   ArcternInterpolation ::LINEAR);
  CHECK_CODE()
  //printDeviceBuffer<uchar>(midMat);

  int topBorderHeight = (input_h - dest_h) >> 1;
  int leftBorderWidth = (input_w - dest_w) >> 1;

  DeviceBuffer *dstMat = new DeviceBuffer ;
  dstMat->resize(input_h, input_w,
                 pRunParam->arcternImage->Channel(),
                 pRunParam->arcternImage->Type());
  const int borderColors[]{m_initPt->borderValue_, m_initPt->borderValue_, \
                      m_initPt->borderValue_, 0};
  m_lastErrorCode = m_nppProcEx.Padding(midMat, *dstMat, \
                    ktopBorderHeight, leftBorderWidth, borderColors);
  CHECK_CODE()

  pRunParam->arcternImage.reset(dstMat);
  //printDeviceBuffer<uchar>(*pRunParam->arcternImage);
  return m_lastErrorCode;
}

int NppPreprocessor::detFace2(NppPreprocessorRunPt *pRunParam){
  const auto input_w = m_initPt->netW_;
  const auto input_h = m_initPt->netH_;
  const int img_width = pRunParam->arcternImage->Width();
  const int img_height = pRunParam->arcternImage->Height();
  auto *dstMat = new DeviceBuffer;
  dstMat->resize(input_h, input_w,
                 pRunParam->arcternImage->Channel(),
                 pRunParam->arcternImage->Type());
  int bottomBorderHeight(0), rightBorderWidth(0);
  int topBorderHeight(0), leftBorderWidth(0);
  if (input_w == input_h) {
    int dest_h(input_h), dest_w(input_w);
    double ratio = std::min(double(input_w)/img_width, double(input_h)/img_height);
    dest_w = int(round(img_width * ratio));
    dest_h = int(round(img_height * ratio));
    double pad_w = (input_w - dest_w)>>1;
    double pad_h = (input_h - dest_h)>>1;
    if (m_initPt->alignMent_ == AlignMent::AlignCenter) {
      topBorderHeight = int(round((pad_h-0.1)));
      bottomBorderHeight = int(round((pad_h+0.1)));
      leftBorderWidth = int(round((pad_w-0.1)));
      rightBorderWidth = int(round((pad_w+0.1)));
    }
    ImgPreprocessEx::DeviceBuffer midMat(dest_h, dest_w,
                                         pRunParam->arcternImage->Channel(),
                                         pRunParam->arcternImage->Type());
    m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage, midMat, LINEAR);
    CHECK_CODE();
    const int borderColors[]{m_initPt->borderValue_, m_initPt->borderValue_, m_initPt->borderValue_, 0};
    m_lastErrorCode = m_nppProcEx.Padding(midMat, *dstMat, topBorderHeight, leftBorderWidth, borderColors);
    CHECK_CODE();
  } else {
    m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage, *dstMat, LINEAR);
    CHECK_CODE();
  }
  pRunParam->arcternImage.reset(dstMat);
  return 0;
}

int NppPreprocessor::detFace(NppPreprocessorRunPt *pRunParam){

  const auto input_w = m_initPt->netW_;
  const auto input_h = m_initPt->netH_;
  const int img_width = pRunParam->arcternImage->Width();
  const int img_height = pRunParam->arcternImage->Height();

  auto *dstMat = new DeviceBuffer;
  dstMat->resize(input_h, input_w,
                 pRunParam->arcternImage->Channel(),
                 pRunParam->arcternImage->Type());
  int bottomBorderHeight(0), rightBorderWidth(0);
  int topBorderHeight(0), leftBorderWidth(0);

  if (input_w == input_h) {

    int new_w = img_width;
    int new_h = img_height;
    if ((static_cast<float>(input_w) / img_width) <
        (static_cast<float>(input_h) / img_height)) {
      new_w = input_w;
      new_h = (img_height * input_w) / img_width;
    } else {
      new_h = input_h;
      new_w = (img_width * input_h) / img_height;
    }

    ImgPreprocessEx::DeviceBuffer midMat(new_h, new_w,
                                         pRunParam->arcternImage->Channel(),
                                         pRunParam->arcternImage->Type());
    m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage, midMat, LINEAR);
    CHECK_CODE();

    const int borderColors[]{m_initPt->borderValue_, m_initPt->borderValue_,\
                   m_initPt->borderValue_, 0};
    m_lastErrorCode = m_nppProcEx.Padding(midMat, *dstMat, (input_h - new_h)\
                   >> 1, (input_w - new_w) >> 1, borderColors);
    CHECK_CODE();

  } else {
    m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage, *dstMat, LINEAR);
    CHECK_CODE();
  }
  pRunParam->arcternImage.reset(dstMat);
  return 0;
}

int NppPreprocessor::calcMatrix( NppPreprocessorRunPt *pRunParam,  double dArray[2][3]) {
  cv::Mat mSrc(2, 2, CV_32FC1);
  cv::Mat mDst(2, 2, CV_32FC1);

  auto *pSrc = mSrc.ptr<float>();
  auto *pDst = mDst.ptr<float>();

  pSrc[0] = m_initPt->lmk_[0].x;
  pSrc[1] = m_initPt->lmk_[0].y;
  pSrc[2] = m_initPt->lmk_[1].x;
  pSrc[3] = m_initPt->lmk_[1].y;

  pDst[0] = pRunParam->m_lmk[0].x;
  pDst[1] = pRunParam->m_lmk[0].y;
  pDst[2] = pRunParam->m_lmk[1].x;
  pDst[3] = pRunParam->m_lmk[1].y;

  cv::Mat M_out = similarTransform(mDst, mSrc);

  cv::Mat M(2, 3, CV_32F);
  for (auto i = 0; i < M.rows; ++i) {
    for (auto j = 0; j < M.cols; ++j) {
      M.at<float>(i, j) = M_out.at<float>(i, j);
    }
  }

  float M_t[6];
  cv::Mat matM_t(2, 3, CV_32F, M_t);
  M.convertTo(matM_t, matM_t.type());
  {
    float D = M_t[0]*M_t[4] - M_t[1]*M_t[3];
    D = D != 0 ? 1.f/D : 0.0f;
    float A11 = M_t[4]*D, A22=M_t[0]*D;
    M_t[0] = A11; M_t[1] *= -D;
    M_t[3] *= -D; M_t[4] = A22;
    float b1 = -M_t[0]*M_t[2] - M_t[1]*M_t[5];
    float b2 = -M_t[3]*M_t[2] - M_t[4]*M_t[5];
    M_t[2] = b1; M_t[5] = b2;
  }
  //double dArray[2][3];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      dArray[i][j] = (double) M_t[i*3+j];
    }
  }
  return 0;
}

int NppPreprocessor::padding(NppPreprocessorRunPt *pRunParam) {
  if(pRunParam->m_paddingParam.size() != 4){
    m_lastErrorCode = -1;
    return m_lastErrorCode;
  }
  auto *dst = new DeviceBuffer ;
  dst->resize(pRunParam->m_paddingParam[0],pRunParam->m_paddingParam[1],
              pRunParam->arcternImage->Channel(),pRunParam->arcternImage->Type());

  const int borderValues[]{m_initPt->borderValue_,m_initPt->borderValue_,\
                m_initPt->borderValue_,m_initPt->borderValue_};
  m_nppProcEx.Padding(*pRunParam->arcternImage,*dst,
                      pRunParam->m_paddingParam[2],
                      pRunParam->m_paddingParam[3],
                      borderValues);
  CHECK_CODE()
  //printDeviceBuffer<uchar>(*dst);
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::cvtColor(NppPreprocessorRunPt *pRunParam) {
  if(pRunParam->m_cvtColorCode == -1) return -1;
  auto * dst = new DeviceBuffer ;
  if (pRunParam->m_cvtColorCode == RGB2GRAY || pRunParam->m_cvtColorCode == BGR2GRAY) {
    dst->resize(pRunParam->arcternImage->Height(), pRunParam->arcternImage->Width(), \
                    1, pRunParam->arcternImage->Type());
  } else {
    dst->resize(pRunParam->arcternImage->Height(), pRunParam->arcternImage->Width(),\
                pRunParam->arcternImage->Channel(), pRunParam->arcternImage->Type());
  }

  m_lastErrorCode = m_nppProcEx.CvtColor(*pRunParam->arcternImage, *dst, \
                  (ColorCvtType) pRunParam->m_cvtColorCode);
  pRunParam->arcternImage.reset(dst);

  ////printDeviceBuffer<uchar>(*pRunParam->arcternImage);
  return m_lastErrorCode;
}

int NppPreprocessor::crop(NppPreprocessorRunPt *pRunParam) {
  auto  * dst = new DeviceBuffer ;
  if (dst->data() == nullptr) {
    dst->resize(pRunParam->m_roi.height, pRunParam->m_roi.width,
               pRunParam->arcternImage->Channel(),
               pRunParam->arcternImage->Type());
  }
  m_lastErrorCode = m_nppProcEx.Crop(*pRunParam->arcternImage, *dst, pRunParam->m_roi);
  m_lastErrorCode = cudaGetLastError();
  // just for debug
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::resize(NppPreprocessorRunPt *pRunParam) {
  auto *dst = new DeviceBuffer;
  dst->resize(m_initPt->netH_,
              m_initPt->netW_,
              pRunParam->arcternImage->Channel(),
              pRunParam->arcternImage->Type());
  m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage,*dst,\
                        ArcternInterpolation::LINEAR);
  CHECK_CODE()
  //printDeviceBuffer<float>(*dst);
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::affine(NppPreprocessorRunPt *pRunParam) {
  double dArray[2][3];
  calcMatrix(pRunParam, dArray);

  auto *dstMat = new DeviceBuffer (m_initPt->netH_, m_initPt->netW_,
                                   pRunParam->arcternImage->Channel(),
                                   //pRunParam->arcternImage->Type());
                                   nvinfer1::DataType::kFLOAT);
  m_lastErrorCode = m_nppProcEx.Affine(*pRunParam->arcternImage, *dstMat, dArray, LINEAR);
  pRunParam->arcternImage.reset(dstMat);

  // just debug
  //printDeviceBuffer<uchar>(*dstMat);
  //printDeviceBuffer<uchar>(*pRunParam->arcternImage);
  return m_lastErrorCode;
}

int NppPreprocessor::splitNormalized(NppPreprocessorRunPt *pRunParam){
  cv::Mat mat(pRunParam->arcternImage->Height(),
              pRunParam->arcternImage->Width(),
              CV_MAKETYPE(CV_8U,pRunParam->arcternImage->Channel()));

  DeviceBuffer mid(pRunParam->arcternImage->Height(),
                   pRunParam->arcternImage->Width(),
                   pRunParam->arcternImage->Channel(),
                   nvinfer1::DataType::kFLOAT);
  m_lastErrorCode = m_nppProcEx.Normalized(*pRunParam->arcternImage,mid,1.0/255,0);
  CHECK_CODE()
  float4 alpha{static_cast<float >(1.0/m_initPt->stdV_[0]),
               static_cast<float >(1.0/m_initPt->stdV_[1]),
               static_cast<float >(1.0/m_initPt->stdV_[2]),0.f};
  float4 beta{ static_cast<float >(-1.0*m_initPt->meanV_[0]/m_initPt->stdV_[0]),
               static_cast<float >(-1.0*m_initPt->meanV_[1]/m_initPt->stdV_[1]),
               static_cast<float >(-1.0*m_initPt->meanV_[2]/m_initPt->stdV_[2]),0.f};
  auto *dst = new DeviceBuffer ;
  dst->resize(mid.Height(),mid.Width(),mid.Channel(),mid.Type());
  m_lastErrorCode = m_nppProcEx.Normalized(mid,*dst,alpha,beta);
  CHECK_CODE()
  //printDeviceBuffer<float>(*dst);
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::normalize(NppPreprocessorRunPt *pRunParam) {
  const auto mean = m_initPt->meanV_.empty() ? m_initPt->mean_ : m_initPt->meanV_[0];
  const auto standDeviation = m_initPt->stdV_.empty() ? m_initPt->std_ : m_initPt->stdV_[0];

  auto * dstMat = new DeviceBuffer ;
  dstMat->resize(pRunParam->arcternImage->Height(), pRunParam->arcternImage->Width(),
                  pRunParam->arcternImage->Channel(), nvinfer1::DataType::kFLOAT);
  m_lastErrorCode = m_nppProcEx.Normalized(*pRunParam->arcternImage, *dstMat, 1.0 / \
                      standDeviation, -1.0 * (mean / standDeviation));

  pRunParam->arcternImage.reset(dstMat);

  ////printDeviceBuffer<float>(*dstMat);

  return m_lastErrorCode;
}

int NppPreprocessor::extractChannel(NppPreprocessorRunPt *pRunParam) {
  auto  * dst = new DeviceBuffer ;
  if (dst->data() == nullptr) {
    dst->resize(pRunParam->arcternImage->Height(), pRunParam->arcternImage->Width(),
                pRunParam->arcternImage->Channel(),
                pRunParam->arcternImage->Type());
  }

  m_lastErrorCode = m_nppProcEx.ExtractChannel(*pRunParam->arcternImage, *dst);

  ////printDeviceBuffer<float>(*dst);
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::merge(NppPreprocessorRunPt *pRunParam){
  auto *dst = new DeviceBuffer ;
  dst->resize(pRunParam->arcternImage->Height(),
              pRunParam->arcternImage->Width(),
              pRunParam->arcternImage->Channel() * 3,
              pRunParam->arcternImage->Type());

  m_lastErrorCode = m_nppProcEx.Merge(*pRunParam->arcternImage,*dst);
  CHECK_CODE();
  //printDeviceBuffer<float>(*dst);
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

int NppPreprocessor::twiceResize(NppPreprocessorRunPt *pRunParam){
  const auto input_w = m_initPt->netW_;
  const auto input_h = m_initPt->netH_;
  auto *dst = new DeviceBuffer;
  dst->resize(input_h,
              input_w,
              pRunParam->arcternImage->Channel(),
              pRunParam->arcternImage->Type());

  if (pRunParam->arcternImage->Width() > input_w * 4 ||
      pRunParam->arcternImage->Height() > input_h * 4) {
    DeviceBuffer mid(input_h*2,input_w*2,
                     pRunParam->arcternImage->Channel(),
                     pRunParam->arcternImage->Type());
    m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage,mid,ArcternInterpolation::LINEAR);
    CHECK_CODE()
    m_lastErrorCode = m_nppProcEx.Resize(mid,*dst,ArcternInterpolation::LINEAR);
    CHECK_CODE()
  } else {
    m_lastErrorCode = m_nppProcEx.Resize(*pRunParam->arcternImage,*dst,ArcternInterpolation::LINEAR);
    CHECK_CODE()
  }
  //printDeviceBuffer<uchar>(*dst);
  pRunParam->arcternImage.reset(dst);
  return m_lastErrorCode;
}

template<typename T>
Mat2Array<T>::Mat2Array():channels(0), step(0),
                          cols(0), rows(0), t(nullptr) {}

template<typename T>
Mat2Array<T>::~ Mat2Array() {
  if (t != nullptr) {
    for (int i = 0; i < rows; i++) {
      if (t[i] != nullptr)
        delete[] t[i];
    }
    delete[] t;
  }
}

template<typename T>
void Mat2Array<T>::init(const Mat &mat) {
  channels = mat.channels();
  cols = mat.cols;
  rows = mat.rows;
  step = cols * channels;
  assert(channels != 0);
  assert(cols != 0);
  assert(rows != 0);
}

template<typename T>
T **Mat2Array<T>::Transform(const Mat &mat) {
  init(mat);

  t = new T *[rows];
  for (int i = 0; i < rows; i++) t[i] = new T[step];

  for (int i = 0; i < rows; i++) {
    const auto *data = mat.ptr<T>(i);
    for (int j = 0; j < cols; j++) {
      for (int k = 0; k < channels; k++) {
        t[i][j * channels + k] = data[j * channels + k];
      }
    }
  }

  return t;
}

template<typename T>
template<int rowSize, int colSize>
void Mat2Array<T>::Transform(const Mat &mat, T array[rowSize][colSize]) {
  init(mat);
  assert(rowSize >= rows);
  assert(colSize >= cols);
  for (int i = 0; i < rows; i++) {
    const auto *data = mat.ptr<T>(i);
    for (int j = 0; j < cols; j++) {
      for (int k = 0; k < channels; k++) {
        array[i][j * channels + k] = data[j * channels + k];
      }
    }
  }
}

}
