/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_HISIFACEIMAGECOLORIMPL_H
#define ARCTERN_HISIFACEIMAGECOLORIMPL_H

#include "../faceImageColorImpl.h"

namespace arctern {
class HisiFaceImageColorImpl : public FaceImageColorImpl {
 public:
  HisiFaceImageColorImpl();
  ~HisiFaceImageColorImpl() override;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_HISIFACEIMAGECOLORIMPL_H
