//
// Created by gh on 2020/12/17.
//

#include "atlasFacialLandmarkImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/Algorithms/facialLandmark/utils/faceLandmarkUtils.h"

namespace arctern{

AtlasFacialLandmarkImpl::AtlasFacialLandmarkImpl(){
   m_modelsInputHeight = m_modelsInputWidth = 128;
}

AtlasFacialLandmarkImpl::~AtlasFacialLandmarkImpl() {
  
}

ErrorType AtlasFacialLandmarkImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasFacialLandmarkImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
    auto r = dynamic_cast<FacialLandmarkResult *>(m_pResult);

  int offset = 0;
  int total = endId - startId;
  float *ret = outputs[0].data();
  int stride = outputs[0].size() / total;

  for (int subId = 0; subId < total; subId++) {
    if (m_landmarkNum == 68) {
      for (int i = 0; i < m_landmarkNum; i++) {

        float tmpY = ret[i * 2 + 1 + offset];
        float tmpX = ret[i * 2 + offset];

        auto point = geneOriLandmark(tmpX, tmpY, m_vFaceRects[startId + subId]);

        r->landmarks_[startId + subId].push_back(point);
      }
    } else {
      for (int i = 0; i < m_landmarkNum; i++) {
        int coor_y = static_cast<int>((*(ret + i * 2 + 1 + offset)) * m_vFaceRects[startId + subId].height + m_vFaceRects[startId + subId].y);
        int coor_x = static_cast<int>((*(ret + i * 2 + offset)) * m_vFaceRects[startId + subId].width + m_vFaceRects[startId + subId].x);
        r->landmarks_[startId + subId].push_back(ArcternPoint({static_cast<float>(coor_x),static_cast<float>(coor_y)}));
      }
    }
    offset += stride;
  }
  return ErrorType::ERR_SUCCESS;
}

cv::Rect AtlasFacialLandmarkImpl::LocalGetROI(const RunParameter *p, int idx){
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  m_landmarkNum = 68;
  if (m_landmarkNum == 68) {
    rect = getFace68(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  } else {
    rect = getFaceTiny(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  }
  m_vFaceRects.push_back(rect);
  return rect;
}

ErrorType AtlasFacialLandmarkImpl::Process(const RunParameter *p,Result *r){
  m_vFaceRects.clear();
  return ArcternAlgorithmEx::Process(p,r);
}

ErrorType AtlasFacialLandmarkImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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
