/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceAbnomalLightImpl.h"

namespace arctern {
ErrorType TvmFaceAbnomalLightImpl::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = FaceAbnomalLightImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 5}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}
}
