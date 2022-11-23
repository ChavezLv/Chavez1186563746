//
// Created by gh on 2021/8/5.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEMASK_RKNN2_RKNN2FACEMASKIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEMASK_RKNN2_RKNN2FACEMASKIMPL_H_

#include "src/Algorithms/faceMask/faceMaskImpl.h"

namespace arctern {
class Rknn2FaceMaskImpl : public FaceMaskImpl {
 public:
  Rknn2FaceMaskImpl();
  ~Rknn2FaceMaskImpl() override;

  ErrorType Init(const InitParameter *p) override;

  std::pair<int, float> calc_mask(const float *data, size_t num_class) override;
};
} // namespace arctern

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEMASK_RKNN2_RKNN2FACEMASKIMPL_H_
