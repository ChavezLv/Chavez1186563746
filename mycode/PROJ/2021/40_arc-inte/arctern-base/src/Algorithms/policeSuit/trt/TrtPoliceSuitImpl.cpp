// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  create:         2021.06.05
// *  email:          kuang.hongshen@intellif.com
// *  modified:
// ****************************************************************************

#include "TrtPoliceSuitImpl.h"
#include "common/algUtil.h"
#include "src/common/common.h"
#include <vector>

namespace arctern {

TrtPoliceSuitImpl::TrtPoliceSuitImpl() {
  m_modelsInputHeight = 50;
  m_modelsInputWidth = 100;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtPoliceSuitImpl::~TrtPoliceSuitImpl() {}

ErrorType
TrtPoliceSuitImpl::GetModelsConfigInfo(const MxnetModels *models) {
  auto model = models->models()->Get(0);

  bool b_ret = try_get_value(*model, "INPUT_W", m_modelsInputWidth);

  b_ret &= try_get_value(*model, "INPUT_H", m_modelsInputHeight);


  return ERR_SUCCESS;
}

ArcternRect TrtPoliceSuitImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &src_rect = pRunParam->rectsV_[idx];
  const cv::Rect oriRect(src_rect.x, src_rect.y, src_rect.width,
                         src_rect.height);
  auto dst_rect = get_chest_roi(oriRect, pRunParam->imageV_[idx].width,
                                pRunParam->imageV_[idx].height);

  ArcternRect rect{(float )dst_rect.x,(float )dst_rect.y,
                   (float )dst_rect.width,(float )dst_rect.height};
  return rect;
}

void TrtPoliceSuitImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

std::pair<PoliceSuitClassify, float>
TrtPoliceSuitImpl::calcSuit(const float *p_data, const size_t class_num) {
  if (nullptr == p_data || class_num <= 0) {
    return std::move(std::make_pair(INVALID_RESULT, 0.f));
  }

  std::vector<float> v_prob(class_num);
  memcpy(v_prob.data(), p_data, class_num * sizeof(float));

  float f_max_val = 0.f;
  int i_max_idx = softmax(v_prob, f_max_val);

  return std::make_pair(PoliceSuitClassify(i_max_idx), f_max_val);
}

cv::Rect TrtPoliceSuitImpl::get_chest_roi(cv::Rect roi, int img_w, int img_h) {
  if (img_w <= 0 || img_h <= 0) {
    return std::move(roi);
  }

  cv::Rect bbox = check_shrink_box(roi, img_w, img_h);
  int crop_xmin = std::max(0, bbox.x - bbox.width);
  int crop_ymin = std::max(0, bbox.y + bbox.height);
  int crop_xmax = std::min(img_w, bbox.x + bbox.width * 2);
  int crop_ymax = std::min(img_h, bbox.y + bbox.height * 2);
  int crop_width = crop_xmax - crop_xmin;
  int crop_height = crop_ymax - crop_ymin;

  if (crop_width < 2) {
    crop_xmin = bbox.x;
    crop_width = 2;
  }

  if (crop_height < 2) {
    crop_ymin = bbox.y;
    crop_height = 2;
  }

  cv::Rect crop_rect({crop_xmin, crop_ymin, crop_width, crop_height});

  return crop_rect;
}

ErrorType TrtPoliceSuitImpl::LocalPostProcess(
    const RunParameter *p, int startId, int endId,
    std::vector<arctern::OutputTensor<float>> &outputs) {
  PoliceSuitResult *p_res = dynamic_cast<PoliceSuitResult *>(m_pResult);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto suit_res = calcSuit(p_data, i_stride);
    p_res->m_vSuitResults[startId + subId].first = suit_res.first;
    p_res->m_vSuitResults[startId + subId].second = suit_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}

} // namespace arctern
