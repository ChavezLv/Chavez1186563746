/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hisiFaceMaskImpl.h"
using arctern::HisiFaceMaskImpl;
using arctern::ErrorType;

HisiFaceMaskImpl::HisiFaceMaskImpl():FaceMaskImpl(){
  // hisi is different
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
  isNormaliized_ = false;
};

HisiFaceMaskImpl::~HisiFaceMaskImpl() {
}

ErrorType HisiFaceMaskImpl::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  return FaceMaskImpl::Init(p);
}