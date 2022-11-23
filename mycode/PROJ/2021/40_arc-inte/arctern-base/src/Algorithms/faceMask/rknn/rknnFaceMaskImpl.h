/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNNFACEMASKIMPL_H
#define ARCTERN_RKNNFACEMASKIMPL_H

#include "src/Algorithms/faceMask/faceMaskImpl.h"

namespace arctern {
class RknnFaceMaskImpl : public FaceMaskImpl {
 public:
  RknnFaceMaskImpl();
  ~RknnFaceMaskImpl() override;

  ErrorType Init(const InitParameter *p) override;

  std::pair<int, float> calc_mask(const float *data, size_t num_class) override;
};
} // namespace arctern

#endif //ARCTERN_RKNNFACEMASKIMPL_H
