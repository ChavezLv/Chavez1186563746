/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.03.02
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceBasicMultiAttrImpl.h"

namespace arctern {
ErrorType TvmFaceBasicMultiAttrImpl::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);
  auto ret = FaceBasicMultiAttrImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 2}, {1, 86}, {1, 3}, {1, 2}, {1, 6}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}
}