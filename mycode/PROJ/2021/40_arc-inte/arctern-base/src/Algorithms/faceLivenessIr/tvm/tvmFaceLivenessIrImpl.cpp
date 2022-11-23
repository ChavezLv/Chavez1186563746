/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceLivenessIrImpl.h"
using arctern::ErrorType;
using arctern::TvmFaceLivenessIrImpl;
ErrorType TvmFaceLivenessIrImpl::Init(const InitParameter *p) {
  // tvm not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  auto ret = FaceLivenessIrImpl::Init(p);

  if(ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 2}});
    engines_[0]->SetOutputShape(output_shapes);
  }
  return ret;
}