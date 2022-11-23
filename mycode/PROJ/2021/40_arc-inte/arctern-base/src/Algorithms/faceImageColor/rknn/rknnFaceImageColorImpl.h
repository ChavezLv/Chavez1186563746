/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.04
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNNFACEIMAGECOLORIMPL_H
#define ARCTERN_RKNNFACEIMAGECOLORIMPL_H

#include "src/Algorithms/faceImageColor/faceImageColorImpl.h"

namespace arctern {
class RknnFaceImageColorImpl : public FaceImageColorImpl {
 public:
  RknnFaceImageColorImpl();
  ~RknnFaceImageColorImpl() override;

  ErrorType Init(const InitParameter *p) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
};
} // namespace arctern

#endif //ARCTERN_RKNNFACEIMAGECOLORIMPL_H
