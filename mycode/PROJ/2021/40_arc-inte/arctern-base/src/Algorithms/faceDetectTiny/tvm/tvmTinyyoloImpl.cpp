/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.3.4
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "tvmTinyyoloImpl.h"

namespace arctern {
ErrorType TvmYoloV3::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = ArcternAlgorithm::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 10647, 6}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}
}