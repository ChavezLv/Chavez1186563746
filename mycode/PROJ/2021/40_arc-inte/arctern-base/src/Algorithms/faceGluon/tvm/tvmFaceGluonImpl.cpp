/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.30
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceGluonImpl.h"

using arctern::ErrorType;
using arctern::TvmFaceGluonImpl;

ErrorType TvmFaceGluonImpl::Init(const arctern::InitParameter *p) {
  assert(p->batchNum_ == 1);

  auto ret = FaceGluonImpl::Init(p);

  if(ErrorType::ERR_SUCCESS == ret) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 512}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}