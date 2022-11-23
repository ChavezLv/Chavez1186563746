//
// Created by gh on 2021/8/5.
//

#include "rknn2FaceQualityImpl.h"

using arctern::ErrorType;
using arctern::Rknn2FaceQualityImpl;

Rknn2FaceQualityImpl::Rknn2FaceQualityImpl() : FaceQualityImpl() {
  isNormaliized_ = false;
  netShape_ = NetworkShape::NHWC;
}

Rknn2FaceQualityImpl::~Rknn2FaceQualityImpl() throw() {
}

ErrorType Rknn2FaceQualityImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);

  return FaceQualityImpl::Init(p);
}

int Rknn2FaceQualityImpl::GetConfigInfo(arctern::MxnetModel *model) {
  outputNum_ = 1;
  isGrayInput_ = false;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
  initPreProcessor();
  return 0;
}
