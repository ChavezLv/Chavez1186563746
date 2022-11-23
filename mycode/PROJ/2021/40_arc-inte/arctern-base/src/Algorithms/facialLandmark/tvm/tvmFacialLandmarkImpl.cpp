/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.26
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFacialLandmarkImpl.h"

using arctern::ErrorType;
using arctern::TvmFacialLandmarkImpl;

ErrorType TvmFacialLandmarkImpl::Init(const arctern::InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = FacialLandmarkImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 136}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}