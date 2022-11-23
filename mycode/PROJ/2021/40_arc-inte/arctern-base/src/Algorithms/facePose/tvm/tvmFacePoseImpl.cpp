/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFacePoseImpl.h"

arctern::ErrorType arctern::TvmFacePoseImpl::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = FacePoseImpl::Init(p);
  if (arctern::ErrorType::ERR_SUCCESS == ret) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 11}, {1, 11}, {1, 11}});
    engines_[0]->SetOutputShape(output_shapes);
  }
  return ret;

}