/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#include "rknnFaceLivenessIrImpl.h"
#include "common/algUtil.h"
using arctern::ErrorType;
using arctern::RknnFaceLivenessIrImpl;

RknnFaceLivenessIrImpl::RknnFaceLivenessIrImpl() : FaceLivenessIrImpl()
{
    isNormaliized_ = false;
    dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

RknnFaceLivenessIrImpl::~RknnFaceLivenessIrImpl() noexcept {
}

ErrorType RknnFaceLivenessIrImpl::Init(const InitParameter *p) {
    // rknn not support batch
    assert(p->batchNum_ == 1);

    return FaceLivenessIrImpl::Init(p);
}

ErrorType RknnFaceLivenessIrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                               std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceLivenessIrResult *>(result_);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    float tmp1 = data[0];
    float tmp2 = data[1];
    tmp1 = std::exp(tmp1);
    tmp2 = std::exp(tmp2);
    float sum = tmp1 + tmp2;
    tmp2 = tmp2 / sum;
    r->faceLiveness_[startId + subId] = tmp2;
  }

  return ErrorType::ERR_SUCCESS;
}
