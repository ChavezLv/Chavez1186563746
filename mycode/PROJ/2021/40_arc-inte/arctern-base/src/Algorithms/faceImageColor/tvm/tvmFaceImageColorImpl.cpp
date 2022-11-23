/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.26
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "tvmFaceImageColorImpl.h"

using arctern::ErrorType;
using arctern::TvmFaceImageColorImpl;

ErrorType TvmFaceImageColorImpl::Init(arctern::InitParameter *p) {
  if(nullptr != p) {
    p->batchNum_ = 1;
  }

  auto ret = FaceImageColorImpl::Init(p);

  if(ErrorType::ERR_SUCCESS == ret) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 2}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}