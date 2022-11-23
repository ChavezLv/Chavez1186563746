/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmFaceDetectRTNImpl.h"

using arctern::ErrorType;
using arctern::TvmFaceDetectRTNImpl;

ErrorType TvmFaceDetectRTNImpl::Init(const arctern::InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = FaceDetectRTNImpl::Init(p);

  if(ret == arctern::ErrorType::ERR_SUCCESS) {

    int tmp = input_h_ * input_w_ * 21 / 512;
    std::vector<std::vector<int64_t> > output_shapes = {{1, tmp, 4}, {1, tmp, 2}, {1, tmp, 10}};

    if(num_output_ == 4) {
      output_shapes.push_back({1, tmp, 1});
    }
    engines_[0]->SetOutputShape(output_shapes);
  }
  return ret;
}