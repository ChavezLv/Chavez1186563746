/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#include "common/algUtil.h"
#include "rknnFaceImageColorImpl.h"

using arctern::ErrorType;
using arctern::RknnFaceImageColorImpl;

RknnFaceImageColorImpl::RknnFaceImageColorImpl() : FaceImageColorImpl() {
  // rknn is different
  isNormaliized_ = false;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
};

RknnFaceImageColorImpl::~RknnFaceImageColorImpl() {
}

ErrorType RknnFaceImageColorImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);
  return FaceImageColorImpl::Init(p);
}

ErrorType RknnFaceImageColorImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                   std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceImageColorResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride0;

    // simple softmax
    float tmp1 = data[0];
    float tmp2 = data[1];
    tmp1 = std::exp(tmp1);
    tmp2 = std::exp(tmp2);
    float sum = tmp1 + tmp2;
    tmp1 = tmp1 / sum;
    r->faceImageColors_[startId + subId] = tmp1;
  }

  return ErrorType::ERR_SUCCESS;
}