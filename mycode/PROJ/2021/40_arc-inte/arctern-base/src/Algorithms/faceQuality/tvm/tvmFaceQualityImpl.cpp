/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceQualityImpl.h"
using arctern::ErrorType;
using arctern::TvmFaceQualityImpl;

ErrorType TvmFaceQualityImpl::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = FaceQualityImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 1}, {1, 2}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}