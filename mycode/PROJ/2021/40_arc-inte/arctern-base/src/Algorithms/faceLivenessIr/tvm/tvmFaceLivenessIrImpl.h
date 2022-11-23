/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_TVMFACELIVENESSIRIMPL_H
#define ARCTERN_BASE_TVMFACELIVENESSIRIMPL_H

#include "../faceLivenessIrImpl.h"

namespace arctern {
class TvmFaceLivenessIrImpl: public FaceLivenessIrImpl {
 public:
  TvmFaceLivenessIrImpl() = default;
  ~TvmFaceLivenessIrImpl() override = default;
  ErrorType Init(const InitParameter *p) override;
};
}


#endif //ARCTERN_BASE_TVMFACELIVENESSIRIMPL_H
