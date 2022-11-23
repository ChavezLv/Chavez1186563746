/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.03.02
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmPersonFeatImpl.h"

namespace arctern {
ErrorType TvmPersonFeatImpl::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = PersonFeatImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 512}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}
}