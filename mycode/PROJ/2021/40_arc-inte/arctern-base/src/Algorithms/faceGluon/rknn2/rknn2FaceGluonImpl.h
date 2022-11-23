//
// Created by gh on 2021/8/5.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_RKNN2_RKNN2FACEGLUONIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_RKNN2_RKNN2FACEGLUONIMPL_H_

#include "src/Algorithms/faceGluon/faceGluonImpl.h"

namespace arctern {
class Rknn2FaceGluonImpl : public FaceGluonImpl {
 public:
  Rknn2FaceGluonImpl();
  ~Rknn2FaceGluonImpl() override;

  ErrorType Init(const InitParameter *p) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
 private:
  std::vector<float> generateFeatureWithChannel1(const float * data, const int stride );
  std::vector<float> generateFeatureWithChannel2(const float * data, const int stride );
};
} // namespace arctern

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_RKNN2_RKNN2FACEGLUONIMPL_H_
