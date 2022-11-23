//
// Created by gh on 2021/8/5.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACENOISEMOTIONBLUR_RKNN2_RKNN2FACENOISEMOTIONBLURIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACENOISEMOTIONBLUR_RKNN2_RKNN2FACENOISEMOTIONBLURIMPL_H_

#include "src/Algorithms/faceNoiseMotionblur/faceNoiseMotionblurImpl.h"

namespace arctern {
class Rknn2FaceNoiseMotionblurImpl : public FaceNoiseMotionblurImpl {
 public:
  Rknn2FaceNoiseMotionblurImpl();
  ~Rknn2FaceNoiseMotionblurImpl() override;

  ErrorType Init(const InitParameter *p) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

  std::pair<int, float> calcALResult(const float *data, size_t num_class) override;
};
} // namespace arctern

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACENOISEMOTIONBLUR_RKNN2_RKNN2FACENOISEMOTIONBLURIMPL_H_
