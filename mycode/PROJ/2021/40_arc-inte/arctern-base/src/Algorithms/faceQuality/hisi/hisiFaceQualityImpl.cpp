/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hisiFaceQualityImpl.h"

using arctern::ErrorType;
using arctern::HisiFaceQualityImpl;

HisiFaceQualityImpl::HisiFaceQualityImpl() :FaceQualityImpl(){
  isNormaliized_ = false;
}

HisiFaceQualityImpl::~HisiFaceQualityImpl() throw() {
}

ErrorType HisiFaceQualityImpl::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  return FaceQualityImpl::Init(p);
}

float HisiFaceQualityImpl::calcResult(const float *data0, const float *data1) {
  if (outputNum_ > 1) {
    auto ret1 =  data1[0] > data1[1] ? 0 : data0[0];
    auto ret2 = data0[0] > data0[1] ? 0 :data1[0];
    return CLS_FIRST_BR_ ? ret1 : ret2;
  } else {
    return data0[0];
  }
}