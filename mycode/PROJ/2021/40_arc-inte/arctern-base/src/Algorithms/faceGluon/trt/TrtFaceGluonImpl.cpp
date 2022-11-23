//
// Created by Admin on 2019/6/9.
//
#include "TrtFaceGluonImpl.h"
#include "src/common/algUtil.h"
#include "src/common/featureUtil.h"
#include "include/autoTime.h"
#include <memory>
#include "nppi.h"

TrtFaceGluonImpl::TrtFaceGluonImpl() {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
  m_modelsInputHeight = 128;
  m_modelsInputWidth = 112;
}
TrtFaceGluonImpl::~TrtFaceGluonImpl() {
  Release();
}

ErrorType TrtFaceGluonImpl::GetModelsConfigInfo(const MxnetModels *models) {

  std::string version = models->version()->str();
  m_version = version;
  std::cout << "version = " << version << std::endl;
  return ERR_SUCCESS;
}

void TrtFaceGluonImpl::InitPreprocessorParam(){
  using gluon::LocalPreProcessV;
  m_type = gluon::getProcessV(m_version);
  m_bMaskWipe = LocalPreProcessV::V4 == m_type ;
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor = std::make_unique<NppPreprocessor>(m_pStream);


  NppPreprocessorInitPt initParam;

  switch (m_type) {
    case LocalPreProcessV::V1:{
      m_modelsInputHeight = 128;
      m_modelsInputWidth = 112;

      initParam.alignFace_ = AlignFace::NoAlignFace;

      m_extHeightScale[0] = 0.12f;
      m_extHeightScale[1] = 0.12f;
      m_extWidthScale[0] = 0.06f;
      m_extWidthScale[1] = 0.06f;

      m_bNeedLandmark = false;
    }
      break;
    case LocalPreProcessV::V2:{
      m_modelsInputHeight = 112;
      m_modelsInputWidth = 96;

      initParam.alignFace_ = AlignFace::NoAlignFace;

      m_extHeightScale[0] = 0.083f;
      m_extHeightScale[1] = 0.083f;
      m_extWidthScale[0] = 0.f;
      m_extWidthScale[1] = 0.f;

      m_bNeedLandmark = false;
    }
      break;
    case LocalPreProcessV::V3:
    case LocalPreProcessV::V4:{
      m_modelsInputHeight = 128;
      m_modelsInputWidth = 112;

      initParam.alignFace_ = AlignFace::AlignFaceWith2point;
      initParam.lmk_.push_back(ArcternPoint{38.2946, 51.6963});
      initParam.lmk_.push_back(ArcternPoint{73.5318, 51.5014});

      m_bNeedLandmark = true;
      break;
    }
    case LocalPreProcessV::V5:
    case LocalPreProcessV::V6:{
      m_modelsInputHeight = 160;
      m_modelsInputWidth = 144;

      m_extHeightScale[0] = 0.12f;
      m_extHeightScale[1] = 0.12f;
      m_extWidthScale[0] = 0.06f;
      m_extWidthScale[1] = 0.06f;

      m_bNeedLandmark = false;
      break;
    }
    default:
      break;
  }
  initParam.netH_ = m_modelsInputHeight;
  initParam.netW_ = m_modelsInputWidth;
  initParam.mean_ = 127.5;
  initParam.std_ = 128.0;
  initParam.borderValue_ =  -127.5 / 128.0;
  assert(m_pPreprocessor);
  auto code = m_pPreprocessor->Init(&initParam);
  assert(code == ErrorType::ERR_SUCCESS);

}

void TrtFaceGluonImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){

  if (m_bNeedLandmark) {
    runParm->m_lmk = m_lmk;
    runParm->sequence_.push_back(OperatorType::affine);
    runParm->sequence_.push_back( OperatorType::cvtColor);
    if(m_bMaskWipe){
      runParm->m_paddingParam.clear();
      runParm->m_paddingParam.reserve(4);
      runParm->m_paddingParam.push_back(static_cast<float >(m_modelsInputHeight));
      runParm->m_paddingParam.push_back(static_cast<float >(m_modelsInputWidth));
      runParm->m_paddingParam.push_back(static_cast<float >(m_modelsInputHeight - (m_modelsInputHeight >> 1)));
      runParm->m_paddingParam.push_back(0.f);
      runParm->sequence_.push_back(OperatorType::padding);
    }
    runParm->sequence_.push_back(OperatorType::extractChannel);
    runParm->sequence_.push_back(OperatorType::normalized);
  } else {

    if(m_type == gluon::LocalPreProcessV::V6){
      runParm->sequence_.push_back(OperatorType::faceFeat);
    }
    else{
      runParm->sequence_.push_back(OperatorType::crop);
    }

    runParm->sequence_.push_back( OperatorType::cvtColor);

    if(m_type != gluon::LocalPreProcessV::V6) {
      runParm->sequence_.push_back(OperatorType::resized);
    }

    if(m_bMaskWipe){
      runParm->m_paddingParam.clear();
      runParm->m_paddingParam.reserve(4);
      runParm->m_paddingParam.push_back(static_cast<float >(m_modelsInputHeight));
      runParm->m_paddingParam.push_back(static_cast<float >(m_modelsInputWidth));
      runParm->m_paddingParam.push_back(static_cast<float >(m_modelsInputHeight - (m_modelsInputHeight >> 1)));
      runParm->m_paddingParam.push_back(0.f);
      runParm->sequence_.push_back(OperatorType::padding);
    }
    runParm->sequence_.push_back(OperatorType::extractChannel);
    runParm->sequence_.push_back(OperatorType::normalized);
  }
}

ErrorType TrtFaceGluonImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {
  assert(p != nullptr);
  assert(startId >= 0);

  auto *ret = dynamic_cast<FaceGluonResult *>(m_pResult);
  if(ret == nullptr) return ERR_NULL_POINT;

  int total = endId - startId;

  int stride = outputs[0].size() / outputs[0].shape()[0];
  int offset = 0;

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + stride * subId;
    std::vector<float> data_cls(stride);
    for (size_t i = 0; i < data_cls.size(); ++i) {
      data_cls[i] = data[i];
    }

    arctern::l2norm(data_cls);
    encrypt_feature(data_cls,ret->features_[startId + subId]);
    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtFaceGluonImpl::LocalGetROI(const RunParameter *p, int idx) {
  //AUTOTIME;
  const auto pRunParam = dynamic_cast<const FaceGluonRunParameter *>(p);
  ArcternRect rect{0.f,0.f,0.f,0.f};
  auto &img = pRunParam->imageV_[idx];
  ///> if need landmark ,then dont need crop
  if (m_bNeedLandmark) {
    auto &landmarks = pRunParam->faceLandmarksV_[idx];
    m_lmk.clear();
    m_lmk = gluon::calcAlignFacePoints((float)img.width, (float)img.height, landmarks);
  }
  else{

    auto &faceRect = pRunParam->rectsV_[idx];
    const cv::Rect oriRect(faceRect.x, faceRect.y, faceRect.width, faceRect.height);
    cv::Rect roi_rect;
    if(m_type == gluon::LocalPreProcessV::V6){
      roi_rect = gluon::getFaceV6(img.width,img.height,oriRect,m_extHeightScale[0],m_extWidthScale[0]);
    }
    else{
      if(m_type == gluon::LocalPreProcessV::V5){
        roi_rect = gluon::getFaceV5(img.width,img.height,oriRect,m_extHeightScale[0],m_extWidthScale[0]);
      }
      else {
        roi_rect = gluon::getFace(img.width, img.height, oriRect, m_extHeightScale, m_extWidthScale);
      }
      roi_rect = check_shrink_box(roi_rect,img.width,img.height);
    }

    rect.x = roi_rect.x;
    rect.y = roi_rect.y;
    rect.width = roi_rect.width;
    rect.height = roi_rect.height;
  }
  if(m_bMaskWipe){
    m_bMaskWipe &= !pRunParam->faceMaskV_.empty();
    if(m_bMaskWipe) m_bMaskWipe &= pRunParam->faceMaskV_[idx];
  }
  return rect;
}
