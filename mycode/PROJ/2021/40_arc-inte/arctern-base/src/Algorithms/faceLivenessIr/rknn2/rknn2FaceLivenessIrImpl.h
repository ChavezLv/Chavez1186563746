//
// Created by gh on 2021/8/5.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACELIVENESSIR_RKNN2_RKNN2FACELIVENESSIRIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACELIVENESSIR_RKNN2_RKNN2FACELIVENESSIRIMPL_H_

#include "src/Algorithms/faceLivenessIr/faceLivenessIrImpl.h"

namespace arctern {
class Rknn2FaceLivenessIrImpl : public FaceLivenessIrImpl {
 public:
  Rknn2FaceLivenessIrImpl();
  ~Rknn2FaceLivenessIrImpl() override;

  ErrorType Init(const InitParameter *p) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
};
} // namespace arctern

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACELIVENESSIR_RKNN2_RKNN2FACELIVENESSIRIMPL_H_
