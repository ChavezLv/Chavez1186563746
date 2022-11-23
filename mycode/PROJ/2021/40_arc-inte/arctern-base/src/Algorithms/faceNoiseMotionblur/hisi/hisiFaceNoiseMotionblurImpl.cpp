/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hisiFaceNoiseMotionblurImpl.h"

using arctern::HisiFaceNoiseMotionblurImpl;
using arctern::ErrorType;

HisiFaceNoiseMotionblurImpl::HisiFaceNoiseMotionblurImpl() {
  isNormaliized_ = false;
}

HisiFaceNoiseMotionblurImpl::~HisiFaceNoiseMotionblurImpl() {
}

ErrorType HisiFaceNoiseMotionblurImpl::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  return FaceNoiseMotionblurImpl::Init(p);
}
