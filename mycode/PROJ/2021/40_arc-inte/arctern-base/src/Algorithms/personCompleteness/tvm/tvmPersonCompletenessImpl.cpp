//
// Created by gh on 2021/7/14.
//

#include "tvmPersonCompletenessImpl.h"

namespace arctern {
ErrorType TvmPersonCompletenessImpl::Init(const InitParameter *p) {
  // tvm not support batch
  assert(p->batchNum_ == 1);

  auto ret = PersonCompletenessImpl::Init(p);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::vector<std::vector<int64_t> > output_shapes({{1, 2}, {1,5}, {16,8}});
    engines_[0]->SetOutputShape(output_shapes);
  }

  return ret;
}
}