/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_SRC_ALGORITHMS_FACELIVENESSIR_RKNN_RKNNFACELIVENESSIR_H_
#define ARCTERN_SRC_ALGORITHMS_FACELIVENESSIR_RKNN_RKNNFACELIVENESSIR_H_

#include "src/Algorithms/faceLivenessIr/faceLivenessIrImpl.h"

namespace arctern {
class RknnFaceLivenessIrImpl : public FaceLivenessIrImpl {
 public:
    RknnFaceLivenessIrImpl();
    ~RknnFaceLivenessIrImpl() override;

    ErrorType Init(const InitParameter *p) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                     std::vector<arctern::Tensor<float> > &outputs) override;
};
} // namespace arctern

#endif //ARCTERN_SRC_ALGORITHMS_FACELIVENESSIR_RKNN_RKNNFACELIVENESSIR_H_
