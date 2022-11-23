/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#include "rknnFaceMaskImpl.h"
#include "common/algUtil.h"

using arctern::ErrorType;
using arctern::RknnFaceMaskImpl;

RknnFaceMaskImpl::RknnFaceMaskImpl() : FaceMaskImpl() {
  // rknn is different
  isNormaliized_ = false;
};

RknnFaceMaskImpl::~RknnFaceMaskImpl() {
}

ErrorType RknnFaceMaskImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);

  return FaceMaskImpl::Init(p);
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> RknnFaceMaskImpl::calc_mask(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(1 - max_idx, max_score);
}