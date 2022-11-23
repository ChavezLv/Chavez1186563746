#include "atlasPersonUmbrellaImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasPersonUmbrellaImpl::AtlasPersonUmbrellaImpl(): m_extScale{0, 0.2, 0, 0}{
   m_modelsInputHeight = 256;
  m_modelsInputWidth = 128;
}

AtlasPersonUmbrellaImpl::~AtlasPersonUmbrellaImpl() {
  
}

ErrorType AtlasPersonUmbrellaImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasPersonUmbrellaImpl::CalculateResult(const float *data, int step) {
  if (nullptr == data || step <= 0) {
    return std::make_pair(-1, 0.f);
  }

  std::vector<float> v_prob(step);
  memcpy(v_prob.data(), data, step * sizeof(float));

  float f_max_val = 0.f;
  int i_max_idx = softmax(v_prob, f_max_val);

  return std::make_pair(i_max_idx, f_max_val);
}

ErrorType AtlasPersonUmbrellaImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   PersonUmbrellaResult *p_res = dynamic_cast<PersonUmbrellaResult *>(m_pResult);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / i_total;
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = CalculateResult(p_data, i_stride);
    p_res->m_vClassifyResults[startId + subId].first = type_res.first;
    p_res->m_vClassifyResults[startId + subId].second = type_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}

cv::Rect AtlasPersonUmbrellaImpl::check_rect(const int width, const int height,
                                   const cv::Rect bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 <= width ? x2 : width);
  auto sy2 = (int) (y2 <= height ? y2 : height);
  y1 = (int) (sy1 - 0.15 * (sy2 - sy1));
  sy1 = (int) (y1 < 0 ? 0 : y1);
  x1 = (int) (sx1 - 0.10 * (sx2 - sx1));
  sx1 = (int) (x1 < 0 ? 0 : x1);
  x2 = (int) (sx2 + 0.10 * (sx2 - sx1));
  sx2 = (int) (x2 <= width ? x2 : width);
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


cv::Rect AtlasPersonUmbrellaImpl::LocalGetROI(const RunParameter *p, int idx)
{
  const auto implRunP = dynamic_cast<const PersonUmbrellaRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_rect(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  return rect;
}

ErrorType AtlasPersonUmbrellaImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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
