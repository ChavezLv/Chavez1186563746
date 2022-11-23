//
// Created by gh on 2021/8/5.
//

#include "rknn2FaceNoiseMotionblurImpl.h"
#include "common/algUtil.h"

using arctern::ErrorType;
using arctern::Rknn2FaceNoiseMotionblurImpl;

Rknn2FaceNoiseMotionblurImpl::Rknn2FaceNoiseMotionblurImpl() :FaceNoiseMotionblurImpl() {
  isNormaliized_ = false;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
  netShape_ = NetworkShape::NHWC;
}

Rknn2FaceNoiseMotionblurImpl::~Rknn2FaceNoiseMotionblurImpl() {
}

ErrorType Rknn2FaceNoiseMotionblurImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);

  return FaceNoiseMotionblurImpl::Init(p);
}

int Rknn2FaceNoiseMotionblurImpl::GetConfigInfo(arctern::MxnetModel *model) {
  int input_dim = 128;
  input_w_ = input_dim;
  input_h_ = input_dim;

  USE_POS_ = 0;
  return 0;
}


std::pair<int, float> Rknn2FaceNoiseMotionblurImpl::calcALResult(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}