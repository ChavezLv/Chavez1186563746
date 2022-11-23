/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.05.8
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceQualityPoseImpl.h"

namespace arctern {
ErrorType TvmFaceQualityPoseImpl::Init(const arctern::InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);
  auto ret = FaceQualityPoseImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 4}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}
}