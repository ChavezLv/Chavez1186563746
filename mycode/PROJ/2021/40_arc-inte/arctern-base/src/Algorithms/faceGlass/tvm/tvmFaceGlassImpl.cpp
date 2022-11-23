/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceGlassImpl.h"

using arctern::ErrorType;
using arctern::TvmFaceGlassImpl;

ErrorType TvmFaceGlassImpl::Init(const InitParameter *p) {
  assert(p->batchNum_ == 1);

  auto ret = FaceGlassImpl::Init(p);

  if (ErrorType::ERR_SUCCESS == ret) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 3}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}