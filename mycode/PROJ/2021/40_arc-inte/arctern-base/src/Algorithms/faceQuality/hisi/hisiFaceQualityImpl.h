/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_HISIFACEQUALITYIMPL_H
#define ARCTERN_HISIFACEQUALITYIMPL_H

#include "../faceQualityImpl.h"

namespace arctern {
class HisiFaceQualityImpl : public FaceQualityImpl {
 public:
  HisiFaceQualityImpl();
  ~HisiFaceQualityImpl();
  ErrorType Init(const InitParameter *p);

  float calcResult(const float *data0, const float *data1) override;
};
}

#endif //ARCTERN_HISIFACEQUALITYIMPL_H
