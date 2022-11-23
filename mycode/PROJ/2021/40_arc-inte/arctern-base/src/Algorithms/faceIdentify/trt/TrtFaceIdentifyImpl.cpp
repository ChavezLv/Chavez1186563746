// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  create:         2021.06.05
// *  email:          kuang.hongshen@intellif.com
// *  modified:
// ****************************************************************************

#include "TrtFaceIdentifyImpl.h"
#include "common/algUtil.h"
#include "src/common/common.h"
#include <vector>

namespace arctern {

TrtFaceIdentifyImpl::TrtFaceIdentifyImpl() : m_aExtScale{0.f, 0.5f, 0.f, 0.1f} {
  m_modelsInputHeight = 255;
  m_modelsInputWidth = 255;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceIdentifyImpl::~TrtFaceIdentifyImpl() {}

ErrorType
TrtFaceIdentifyImpl::GetModelsConfigInfo(const MxnetModels *models) {
  auto model = models->models()->Get(0);


  bool b_ret = try_get_value(*model, "INPUT_W", m_modelsInputWidth);

  b_ret &= try_get_value(*model, "INPUT_H", m_modelsInputHeight);

  return ERR_SUCCESS;
}


ArcternRect TrtFaceIdentifyImpl::LocalGetROI(const RunParameter *p, int idx) {
  auto &src_rect = p->rectsV_[idx];
  const cv::Rect oriRect(src_rect.x, src_rect.y, src_rect.width,src_rect.height);
  auto dst_rect = getFace(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  ArcternRect rect;
  rect.x = dst_rect.x;
  rect.y = dst_rect.y;
  rect.width = dst_rect.width;
  rect.height = dst_rect.height;
  return rect;
}

void TrtFaceIdentifyImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}
ErrorType TrtFaceIdentifyImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
    std::vector<arctern::OutputTensor<float>> &outputs) {

  auto p_res = dynamic_cast<FaceIdentifyResult *>(m_pResult);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = calcType(p_data, i_stride);
    p_res->m_vTypeResults[startId + subId].first = type_res.first;
    p_res->m_vTypeResults[startId + subId].second = type_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}

std::pair<int, float> TrtFaceIdentifyImpl::calcType(const float *p_data,
                                                    const size_t class_num) {
  if (nullptr == p_data || class_num <= 0) {
    return std::make_pair(-1, 0.f);
  }

  std::vector<float> v_prob(class_num);
  memcpy(v_prob.data(), p_data, class_num * sizeof(float));

  float f_max_val = 0.f;
  int i_max_idx = softmax(v_prob, f_max_val);

  return std::make_pair(i_max_idx, f_max_val);
}

cv::Rect TrtFaceIdentifyImpl::getFace(const int width, const int height,
                                      const cv::Rect bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * m_aExtScale.at(1);
  y2 = y2 + h * m_aExtScale.at(3);

  float by1 = std::min(std::max(0, static_cast<int>(y1)), height);
  float by2 = std::min(std::max(0, static_cast<int>(y2)), height);

  float hh = by2 - by1 + 1;
  float ww = hh;

  if (ww > w) {
    int wx = (int)(ww - w) / 2;
    x1 = x1 - wx;
    x2 = x2 + wx;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1)), width);
  float bx2 = std::min(std::max(0, static_cast<int>(x2)), width);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}

} // namespace arctern
