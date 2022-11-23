/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceHatImpl.h"

using arctern::ErrorType;
using arctern::TvmFaceHatImpl;

ErrorType TvmFaceHatImpl::Init(const arctern::InitParameter *p) {
  assert(p->batchNum_ == 1);

  auto ret = FaceHatImpl::Init(p);

  if(ErrorType::ERR_SUCCESS == ret) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 2}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;

}