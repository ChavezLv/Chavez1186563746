/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2021.05.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "tvmPersonQualityImpl.h"

namespace arctern {
ErrorType TvmPersonQualityImpl::Init(const arctern::InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);
  auto ret = PersonQualityImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 1}, {1,2}, {1,2}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}
}