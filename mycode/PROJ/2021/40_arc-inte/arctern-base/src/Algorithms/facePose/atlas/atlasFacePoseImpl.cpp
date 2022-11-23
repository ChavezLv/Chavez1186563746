//
// Created by gh on 2020/12/17.
//

#include "atlasFacePoseImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasFacePoseImpl::AtlasFacePoseImpl(){
  m_modelsInputHeight = 64;
  m_modelsInputWidth = 64;
}

AtlasFacePoseImpl::~AtlasFacePoseImpl() {
  
}

ErrorType AtlasFacePoseImpl::GetModelsConfigInfo(const TrtModels* models){
  
  return ErrorType::ERR_SUCCESS;
}

cv::Rect  AtlasFacePoseImpl::GetFace(int width, int height, const cv::Rect &bbox) {
  float scale = 1.1f;
  int w = bbox.width;
  int h = bbox.height;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;

  int scaled_w = static_cast<int>(w * scale + 0.5);
  int scaled_h = static_cast<int>(h * scale + 0.5);

  x1 = x1 - (scaled_w - w) / 2;
  y1 = y1 - (scaled_h - h) / 2;

  int roundingX1 = static_cast<int>(x1 + 0.5);
  int roundingY1 = static_cast<int>(y1 + 0.5);

  x1 = std::min(width - 1, std::max(0, roundingX1));
  y1 = std::min(height - 1, std::max(0, roundingY1));

  float x2 = std::min(width - 1, std::max(0, roundingX1 + scaled_w));
  float y2 = std::min(height - 1, std::max(0, roundingY1 + scaled_h));

  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}


ErrorType AtlasFacePoseImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<FacePoseResult *>(m_pResult);

  int total = endId - startId;

  int stride = outputs[0].size() / total;
  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride;
    const float *data1 = outputs[1].data() + subId * stride;
    const float *data2 = outputs[2].data() + subId * stride;
    r->facePose_[startId + subId].pitch = CalculateResult(data0, stride);
    r->facePose_[startId + subId].yaw = CalculateResult(data1, stride);
    r->facePose_[startId + subId].roll = CalculateResult(data2, stride);
  }

  return ErrorType::ERR_SUCCESS;
}

FacePoseLevel AtlasFacePoseImpl::CalculateResult(const float *data, int step) {
  int distance = std::distance(data, std::max_element(data, data + step));
  return arctern::FacePoseLevel(distance);
}

cv::Rect AtlasFacePoseImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];

  auto rect = check_shrink_box({faceRect.x,faceRect.y,faceRect.width,faceRect.height}, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = AtlasFacePoseImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  return rect;
}

ErrorType AtlasFacePoseImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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
