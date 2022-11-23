/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.29
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "hisiFacialLandmarkImpl.h"
using arctern::HisiFacialLandmarkImpl;
using arctern::ErrorType;

HisiFacialLandmarkImpl::HisiFacialLandmarkImpl():FacialLandmarkImpl() {
  // hisi is different
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
  isNormaliized_ = false;
}

HisiFacialLandmarkImpl::~HisiFacialLandmarkImpl() {
}

ErrorType HisiFacialLandmarkImpl::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  return FacialLandmarkImpl::Init(p);
}