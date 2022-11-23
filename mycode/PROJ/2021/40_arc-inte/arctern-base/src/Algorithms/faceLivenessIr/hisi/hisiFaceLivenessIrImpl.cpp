/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hisiFaceLivenessIrImpl.h"

using arctern::ErrorType;
using arctern::HisiFaceLivenessIrImpl;

HisiFaceLivenessIrImpl::HisiFaceLivenessIrImpl() :FaceLivenessIrImpl(){
  isNormaliized_ = false;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_GRAY;
}

HisiFaceLivenessIrImpl::~HisiFaceLivenessIrImpl() throw() {
}

ErrorType HisiFaceLivenessIrImpl::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  return FaceLivenessIrImpl::Init(p);
}