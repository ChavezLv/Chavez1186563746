/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#include "rknnFaceGluonImpl.h"
#include "src/Engin/src/rknn/rknnengin.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"
using arctern::ErrorType;
using arctern::RknnFaceGluonImpl;

RknnFaceGluonImpl::RknnFaceGluonImpl() {
  isNormaliized_ = false;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

RknnFaceGluonImpl::~RknnFaceGluonImpl() {
}

ErrorType RknnFaceGluonImpl::Init(const InitParameter *p) {
  // rknn not support batch

  assert(p->batchNum_ == 1);

  return FaceGluonImpl::Init(p);
}

ErrorType RknnFaceGluonImpl::LocalPostProcess(const arctern::RunParameter *p,
                                              int startId,
                                              int endId,
                                              std::vector<arctern::Tensor<float> > &outputs) {

  assert(p != nullptr);
  assert(startId >= 0);
  auto r = dynamic_cast<FaceGluonResult *>(result_);

  int total = endId - startId;
  int chan = outputs[0].shape()[3];   //because some model is 1*1*512*2
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + stride * subId;
    auto data_cls = chan == 2 ? generateFeatureWithChannel2(data,stride) : generateFeatureWithChannel1(data,stride);
    r->features_[startId + subId] = gluon::encryptFeature(data_cls);
  }

  return ErrorType::ERR_SUCCESS;
}

std::vector<float> RknnFaceGluonImpl::generateFeatureWithChannel1(const float *data, const int stride) {
  std::vector<float> data_cls(stride);
  for (size_t i = 0; i < data_cls.size(); ++i) {
    data_cls[i] = data[i];
  }

  arctern::l2norm(data_cls);
  return data_cls;
}

std::vector<float> RknnFaceGluonImpl::generateFeatureWithChannel2(const float *data, const int stride) {
  int halfStride = stride / 2;
  std::vector<float> data_cls1(halfStride);
  std::vector<float> data_cls2(halfStride);
  for (size_t i = 0; i < halfStride; ++i) {
    data_cls1[i] = data[i];
    data_cls2[i] = data[i + halfStride];
  }
  arctern::l2norm(data_cls1);
  arctern::l2norm(data_cls2);

  std::vector<float> data_cls(halfStride);
  for (int i = 0; i < halfStride; i++) {
    data_cls[i] = data_cls1[i] + data_cls2[i];
  }
  arctern::l2norm(data_cls);
  return data_cls;

}


