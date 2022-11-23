/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceExpressionImpl.h"

using arctern::ErrorType;
using arctern::TvmFaceExpressionImpl;

ErrorType TvmFaceExpressionImpl::Init(const InitParameter *p) {
  assert(p->batchNum_ == 1);
  auto ret = FaceExpressionImpl::Init(p);

  if (ErrorType::ERR_SUCCESS == ret) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 7}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}