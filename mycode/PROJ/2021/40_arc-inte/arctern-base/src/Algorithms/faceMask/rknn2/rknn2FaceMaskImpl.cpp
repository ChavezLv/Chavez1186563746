//
// Created by gh on 2021/8/5.
//

#include "rknn2FaceMaskImpl.h"
#include "common/algUtil.h"

using arctern::ErrorType;
using arctern::Rknn2FaceMaskImpl;

Rknn2FaceMaskImpl::Rknn2FaceMaskImpl() : FaceMaskImpl() {
  // rknn is different
  isNormaliized_ = false;
  netShape_ = NetworkShape::NHWC;
  input_h_ = 128;
  input_w_ = 112;

};

Rknn2FaceMaskImpl::~Rknn2FaceMaskImpl() {
}

ErrorType Rknn2FaceMaskImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);
  return FaceMaskImpl::Init(p);
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> Rknn2FaceMaskImpl::calc_mask(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(1 - max_idx, max_score);
}