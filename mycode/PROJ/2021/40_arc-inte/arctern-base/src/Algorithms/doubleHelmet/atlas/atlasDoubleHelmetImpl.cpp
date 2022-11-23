//
// Created by gh on 2020/12/17.
//


#include "atlasDoubleHelmetImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
namespace arctern{

AtlasDoubleHelmetImpl::AtlasDoubleHelmetImpl(){
   m_modelsInputWidth = 192;
   m_modelsInputHeight = 384;
}

AtlasDoubleHelmetImpl::~AtlasDoubleHelmetImpl() {
  
}

ErrorType AtlasDoubleHelmetImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasDoubleHelmetImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

ErrorType AtlasDoubleHelmetImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  auto r = dynamic_cast<DoubleHelmetResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / total;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto helmet = CalculateResult(data0, stride0);

    r->m_vClassifyResults[startId + subId].first = helmet.first;
    r->m_vClassifyResults[startId + subId].second = helmet.second;
  }

  return ErrorType::ERR_SUCCESS;
}

cv::Rect AtlasDoubleHelmetImpl::check_rect(const int& width, const int& height, const cv::Rect &bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;

  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 <= width ? x2 : width);
  auto sy2 = (int) (y2 <= height ? y2 : height);

  x1 = (int) (sx1 - int(0.00 * (sx2 - sx1)));
  y1 = (int) (sy1 - int(0.30 * (sy2 - sy1)));
  x2 = (int) (sx2 + int(0.00 * (sx2 - sx1)));
  y2 = (int) (sy2 - int(0.00 * (sy2 - sy1)));

  sx1 = (int) (x1 < 0 ? 0 : x1);
  sy1 = (int) (y1 < 0 ? 0 : y1);
  sx2 = (int) (x2 <= width ? x2 : width);
  sy2 = (int) (y2 <= height ? y2 : height);

  auto ew  = (int) (sx2 - sx1);
  auto eh  = (int) (sy2 - sy1);
  if (ew > 0 && eh > 0) {
    cv::Rect vehicle_rect = cv::Rect(sx1, sy1, ew, eh);
    return vehicle_rect;
  }
  else {
    cv::Rect vehicle_rect = cv::Rect(0, 0, width, height);
    return vehicle_rect;
  }
}

cv::Rect AtlasDoubleHelmetImpl::LocalGetROI(const RunParameter *p, int idx)
{
  const auto implRunP = dynamic_cast<const DoubleHelmetRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto  rect = check_rect(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  return rect;
}

ErrorType AtlasDoubleHelmetImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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
