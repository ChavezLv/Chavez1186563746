/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_HISIFACENOISEMOTIONBLURIMPL_H
#define ARCTERN_HISIFACENOISEMOTIONBLURIMPL_H

#include "../faceNoiseMotionblurImpl.h"

namespace arctern {
class HisiFaceNoiseMotionblurImpl : public FaceNoiseMotionblurImpl {
 public:
  HisiFaceNoiseMotionblurImpl();
  ~HisiFaceNoiseMotionblurImpl() override;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_HISIFACENOISEMOTIONBLURIMPL_H
