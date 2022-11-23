/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNNFACEGLUONIMPL_H
#define ARCTERN_RKNNFACEGLUONIMPL_H

#include "src/Algorithms/faceGluon/faceGluonImpl.h"

namespace arctern {
class RknnFaceGluonImpl : public FaceGluonImpl {
 public:
  RknnFaceGluonImpl();
  ~RknnFaceGluonImpl() override;

  ErrorType Init(const InitParameter *p) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
 private:
  std::vector<float> generateFeatureWithChannel1(const float * data, const int stride );
  std::vector<float> generateFeatureWithChannel2(const float * data, const int stride );
};
} // namespace arctern

#endif //ARCTERN_RKNNFACEGLUONIMPL_H
