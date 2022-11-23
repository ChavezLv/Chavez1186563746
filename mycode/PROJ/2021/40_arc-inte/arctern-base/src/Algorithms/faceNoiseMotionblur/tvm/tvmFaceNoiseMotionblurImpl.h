/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_TVMFACENOISEMOTIONBLURIMPL_H
#define ARCTERN_BASE_TVMFACENOISEMOTIONBLURIMPL_H
#include "../faceNoiseMotionblurImpl.h"

namespace arctern {
class TvmFaceNoiseMotionblurImpl : public FaceNoiseMotionblurImpl {
 public:
  TvmFaceNoiseMotionblurImpl() = default;
  ~TvmFaceNoiseMotionblurImpl() override = default;
  ErrorType Init(const InitParameter *p) override;
};
}  //  namespace arctern

#endif //ARCTERN_BASE_TVMFACENOISEMOTIONBLURIMPL_H
