/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.02
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNNFACIALLANDMARKIMPL_H
#define ARCTERN_RKNNFACIALLANDMARKIMPL_H

#include "src/Algorithms/facialLandmark/facialLandmarkImpl.h"

namespace arctern {
class RknnFacialLandmarkImpl : public FacialLandmarkImpl {
 public:
    RknnFacialLandmarkImpl();
    ~RknnFacialLandmarkImpl() override;

    ErrorType Init(const InitParameter *p) override;
    ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
            std::vector<arctern::Tensor<float> > &outputs) override;
};
} // namespace arctern

#endif //ARCTERN_RKNNFACIALLANDMARKIMPL_H
