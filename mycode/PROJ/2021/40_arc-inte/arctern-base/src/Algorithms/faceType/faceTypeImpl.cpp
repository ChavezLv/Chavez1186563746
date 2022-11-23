// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "faceTypeImpl.h"
#include "common/algUtil.h"
#include "faceType.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include <string.h>

using namespace arctern;

faceTypeImpl::faceTypeImpl()
    : m_aEextScale{-1.0f, 0.2f, -1.0f, 0.1f} {
  input_w_ = 128;
  input_h_ = 128;
}

faceTypeImpl::~faceTypeImpl() { faceTypeImpl::Release(); }

ErrorType
faceTypeImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                               std::vector<arctern::Tensor<float>> &outputs) {
  FaceTypeResult *p_res = dynamic_cast<FaceTypeResult *>(result_);

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

std::pair<int, float> faceTypeImpl::calcType(const float *p_data,
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

cv::Rect faceTypeImpl::getFace(const int &width, const int &height,
                               const cv::Rect &bbox) {
  int w = bbox.width + 1;
  int h = bbox.height + 1;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;

  y1 = y1 - h * m_aEextScale[1];
  y2 = y2 + h * m_aEextScale[3];

  float by1 = std::min(std::max(0, static_cast<int>(y1 + 0.5)), height - 1);
  float by2 = std::min(std::max(0, static_cast<int>(y2 + 0.5)), height - 1);
  float hh = by2 - by1 + 1;
  float ww = hh;

  if (ww > w) {
    x1 = x1 - (ww - w) / 2.f;
    x2 = x2 + (ww - w) / 2.f;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1 + 0.5f)), width - 1);
  float bx2 = std::min(std::max(0, static_cast<int>(x2 + 0.5f)), width - 1);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}

ArcternRect faceTypeImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto p_run_param = dynamic_cast<const FaceTypeRunParameter *>(p);
  auto &arct_rect = p_run_param->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect bbox = getFace(p->imageV_[idx].width, p->imageV_[idx].height, ori_rect);
  ArcternRect result;
  result.x = bbox.x;
  result.y = bbox.y;
  result.width = bbox.width;
  result.height = bbox.height;
  return result;
}
void faceTypeImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}