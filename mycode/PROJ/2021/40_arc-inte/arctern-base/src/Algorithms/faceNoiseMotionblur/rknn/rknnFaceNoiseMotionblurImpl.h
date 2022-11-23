/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.02
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNNFACENOISEMOTIONBLURIMPL_H
#define ARCTERN_RKNNFACENOISEMOTIONBLURIMPL_H

#include "src/Algorithms/faceNoiseMotionblur/faceNoiseMotionblurImpl.h"

namespace arctern {
class RknnFaceNoiseMotionblurImpl : public FaceNoiseMotionblurImpl {
 public:
  RknnFaceNoiseMotionblurImpl();
  ~RknnFaceNoiseMotionblurImpl() override;

  ErrorType Init(const InitParameter *p) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

  std::pair<int, float> calcALResult(const float *data, size_t num_class) override;
};
} // namespace arctern

#endif //ARCTERN_RKNNFACENOISEMOTIONBLURIMPL_H
