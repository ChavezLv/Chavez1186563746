//
// Created by gh on 2020/12/17.
//

#include "atlasFaceGluonImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasFaceGluonImpl::AtlasFaceGluonImpl(){
  m_modelsInputHeight = 128;
  m_modelsInputWidth = 112;
}

AtlasFaceGluonImpl::~AtlasFaceGluonImpl() {
  
}

ErrorType AtlasFaceGluonImpl::GetModelsConfigInfo(const TrtModels* models){
  using gluon::LocalPreProcessV;
  //m_type = gluon::getProcessV(m_version);
  if(m_version.empty()) m_type = LocalPreProcessV::V1;
  m_bMaskWipe = LocalPreProcessV::V4 == m_type ;

  switch (m_type) {
    case LocalPreProcessV::V1:{
      m_modelsInputHeight = 128;
      m_modelsInputWidth = 112;

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

      m_bNeedLandmark = true;
      break;
    }
    case LocalPreProcessV::V5:{
      m_modelsInputHeight = 160;
      m_modelsInputWidth = 144;

      m_extHeightScale[0] = 0.12f;
      m_extHeightScale[1] = 0.12f;
      m_extWidthScale[0] = 0.06f;
      m_extWidthScale[1] = 0.06f;

      m_bNeedLandmark = false;
      break;
    }
      break;
    default:
      break;
  }

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasFaceGluonImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  assert(p != nullptr);
  assert(startId >= 0);

  auto *ret = dynamic_cast<FaceGluonResult *>(m_pResult);
  if(ret == nullptr) return ERR_NULL_POINT;

  int total = endId - startId;

  int stride = outputs[0].size() / total;
  int offset = 0;

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + stride * subId;
    std::vector<float> data_cls(stride);
    for (size_t i = 0; i < data_cls.size(); ++i) {
      data_cls[i] = data[i];
    }

    arctern::l2norm(data_cls);
    //std::copy(data_cls.begin(), data_cls.end(), std::ostream_iterator<float>(std::cout, " "));
    //std::cout<<std::endl;
    encrypt_feature(data_cls,ret->features_[startId + subId]);
    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}



cv::Rect AtlasFaceGluonImpl::LocalGetROI(const RunParameter *p, int idx){
  const auto pRunParam = dynamic_cast<const FaceGluonRunParameter *>(p);
  cv::Rect rect{0,0,0,0};
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

    if(m_type == gluon::LocalPreProcessV::V5){
      rect = gluon::getFaceV5(img.width,img.height,oriRect,m_extHeightScale[0],m_extWidthScale[0]);
    }
    else{
      rect = gluon::getFace(img.width, img.height, oriRect, m_extHeightScale, m_extWidthScale);
    }
    rect = check_shrink_box(rect,img.width,img.height);

   
  }
  if(m_bMaskWipe){
    m_bMaskWipe &= ~pRunParam->faceMaskV_.empty();
    if(m_bMaskWipe) m_bMaskWipe &= pRunParam->faceMaskV_[idx];
  }
  return rect;
}

ErrorType AtlasFaceGluonImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
std::vector<DvppDataInfo>& vOutputInfos,
std::vector<cv::Rect>& vRects) {
  int size = vInputInfo.size();
  std::vector<DvppCropInputInfo> vCropInputInfo(size);
  for(int i = 0; i < size; ++i){
    vCropInputInfo[i].dataInfo = vInputInfo[i];
    vCropInputInfo[i].roi.left = vRects[i].x;
    vCropInputInfo[i].roi.up = vRects[i].y;
    vCropInputInfo[i].roi.right = vRects[i].x + vRects[i].width - 1;
    vCropInputInfo[i].roi.down = vRects[i].y + vRects[i].height - 1;
  }

  auto ret = m_pPreprocessor->CombineCropResizeBatchProcess(vCropInputInfo,vOutputInfos,true);

  if (ret != APP_ERR_OK) {
    LOGASDError << "failed to do altas preprocess";
    m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
  }
  return m_lastErrorCode;
}


}
