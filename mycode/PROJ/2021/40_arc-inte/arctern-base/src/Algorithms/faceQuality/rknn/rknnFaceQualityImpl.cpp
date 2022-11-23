/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.02
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#include "rknnFaceQualityImpl.h"

using arctern::ErrorType;
using arctern::RknnFaceQualityImpl;

RknnFaceQualityImpl::RknnFaceQualityImpl() : FaceQualityImpl() {
  isNormaliized_ = false;
}

RknnFaceQualityImpl::~RknnFaceQualityImpl() throw() {
}

ErrorType RknnFaceQualityImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);

  return FaceQualityImpl::Init(p);
}

int RknnFaceQualityImpl::GetConfigInfo(arctern::MxnetModel *model) {
  outputNum_ = 1;
  isGrayInput_ = false;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
  return 0;
}

