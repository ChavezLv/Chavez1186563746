/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.29
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_HISIFACIALLANDMARKIMPL_H
#define ARCTERN_HISIFACIALLANDMARKIMPL_H

#include "../facialLandmarkImpl.h"

namespace arctern {
class HisiFacialLandmarkImpl : public FacialLandmarkImpl {
 public:
  HisiFacialLandmarkImpl();
  ~HisiFacialLandmarkImpl() override;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_HISIFACIALLANDMARKIMPL_H
