//
// Created by gh on 2020/12/17.
//

#include "atlasFaceSmokeImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/Algorithms/faceSmoke/utils/faceSmokeUtil.h"

namespace arctern{

AtlasFaceSmokeImpl::AtlasFaceSmokeImpl(){
   m_modelsInputHeight = m_modelsInputWidth = 255;
}

AtlasFaceSmokeImpl::~AtlasFaceSmokeImpl() {
  
}

ErrorType AtlasFaceSmokeImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasFaceSmokeImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<FaceSmokeResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / total;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto phone = CalculateResult(data0, stride0);

    r->faceSmokes_[startId + subId].smokeStatus = SmokeStatus(phone.first);
    r->faceSmokes_[startId + subId].statusScore = phone.second;
  }

  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasFaceSmokeImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

cv::Rect AtlasFaceSmokeImpl::LocalGetROI(const RunParameter *p, int idx){
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
 
  auto rect = smoke::getFace(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);

  return rect;
}

ErrorType AtlasFaceSmokeImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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
