/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceNoiseMotionblurImpl.h"

using arctern::ErrorType;
using arctern::TvmFaceNoiseMotionblurImpl;

ErrorType TvmFaceNoiseMotionblurImpl::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = FaceNoiseMotionblurImpl::Init(p);

  if(ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 3}});
    engines_[0]->SetOutputShape(output_shapes);
  }
  return ret;
}