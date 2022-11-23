/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hisiFaceImageColorImpl.h"
using arctern::HisiFaceImageColorImpl;
using arctern::ErrorType;

HisiFaceImageColorImpl::HisiFaceImageColorImpl():FaceImageColorImpl(){
  // hisi is different
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
  isNormaliized_ = false;
};

HisiFaceImageColorImpl::~HisiFaceImageColorImpl() {
}

ErrorType HisiFaceImageColorImpl::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  return FaceImageColorImpl::Init(p);
}