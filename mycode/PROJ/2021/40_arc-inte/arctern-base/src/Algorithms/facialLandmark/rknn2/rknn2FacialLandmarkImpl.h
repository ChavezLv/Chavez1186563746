/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.02
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNN2FACIALLANDMARKIMPL_H
#define ARCTERN_RKNN2FACIALLANDMARKIMPL_H

#include "../facialLandmarkImpl.h"

namespace arctern {
class Rknn2FacialLandmarkImpl : public FacialLandmarkImpl {
 public:
  Rknn2FacialLandmarkImpl();
    ~Rknn2FacialLandmarkImpl() override;

    ErrorType Init(const InitParameter *p) override;
    ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
            std::vector<arctern::Tensor<float> > &outputs) override;
};
} // namespace arctern

#endif //ARCTERN_RKNN2FACIALLANDMARKIMPL_H
