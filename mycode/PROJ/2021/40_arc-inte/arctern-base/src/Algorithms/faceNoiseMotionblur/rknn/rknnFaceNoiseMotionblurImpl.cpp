/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.02
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#include "rknnFaceNoiseMotionblurImpl.h"
#include "common/algUtil.h"

using arctern::ErrorType;
using arctern::RknnFaceNoiseMotionblurImpl;

RknnFaceNoiseMotionblurImpl::RknnFaceNoiseMotionblurImpl() :FaceNoiseMotionblurImpl() {
    isNormaliized_ = false;
    dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

RknnFaceNoiseMotionblurImpl::~RknnFaceNoiseMotionblurImpl() {
}

ErrorType RknnFaceNoiseMotionblurImpl::Init(const InitParameter *p) {
    // rknn not support batch
    assert(p->batchNum_ == 1);

    return FaceNoiseMotionblurImpl::Init(p);
}

int RknnFaceNoiseMotionblurImpl::GetConfigInfo(arctern::MxnetModel *model) {
    int input_dim = 128;
    input_w_ = input_dim;
    input_h_ = input_dim;

    USE_POS_ = 0;
    return 0;
}


std::pair<int, float> RknnFaceNoiseMotionblurImpl::calcALResult(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}