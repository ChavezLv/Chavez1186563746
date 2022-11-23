//
// Created by gh on 2020/12/17.
//

#include "atlasFaceTypeImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasFaceTypeImpl::AtlasFaceTypeImpl(): m_extScale{-1.0f, 0.5f, -1.0f, 0.1f}{
   m_modelsInputHeight = m_modelsInputWidth = 128;
}

AtlasFaceTypeImpl::~AtlasFaceTypeImpl() {
  
}

ErrorType AtlasFaceTypeImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasFaceTypeImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  auto r = dynamic_cast<FaceTypeResult *>(m_pResult);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / i_total;
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = CalculateResult(p_data, i_stride);
    r->m_vTypeResults[startId + subId].first = type_res.first;
    r->m_vTypeResults[startId + subId].second = type_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasFaceTypeImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::get_label(prob, max_score,0.2434);
  return std::make_pair(max_idx, max_score);
}

cv::Rect AtlasFaceTypeImpl::GetFace(const int &width, const int &height,const cv::Rect &bbox) {

  int w = bbox.width + 1;
  int h = bbox.height + 1;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;

  y1 = y1 - h * m_extScale[1];
  y2 = y2 + h * m_extScale[3];
  float by1 = std::min(std::max(0, static_cast<int>((int)(y1 + 0.5))), height - 1);
  float by2 = std::min(std::max(0, static_cast<int>((int)(y2 + 0.5))), height - 1);
  float hh = by2 - by1 + 1;
  float ww = hh;
  if (ww > w) {
    x1 = x1 - (ww - w) / 2;
    x2 = x2 + (ww - w) / 2;
  }
  float bx1 = std::min(std::max(0, static_cast<int>((int)(x1 + 0.5))), width - 1);
  float bx2 = std::min(std::max(0, static_cast<int>((int)(x2 + 0.5))), width - 1);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}


cv::Rect AtlasFaceTypeImpl::LocalGetROI(const RunParameter *p, int idx){
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect,p->imageV_[idx].width, p->imageV_[idx].height);
  rect = GetFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  return rect;
}

ErrorType AtlasFaceTypeImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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
