//
// Created by gh on 2021/7/14.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_TVM_TVMPERSONCOMPLETENESS_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_TVM_TVMPERSONCOMPLETENESS_H_

#include "../personCompletenessImpl.h"
namespace arctern {
class TvmPersonCompletenessImpl : public PersonCompletenessImpl {
 public:
  TvmPersonCompletenessImpl() = default;
  ~TvmPersonCompletenessImpl() noexcept = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_TVM_TVMPERSONCOMPLETENESS_H_
