/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.03.1
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_TVM_TVMAGEGENDERIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_TVM_TVMAGEGENDERIMPL_H_

#include "../ageGenderImpl.h"

namespace arctern {

class TvmAgeGenderImpl : public AgeGenderImpl {
 public:
  TvmAgeGenderImpl() = default;
  ~TvmAgeGenderImpl() noexcept = default;
  ErrorType Init(const InitParameter *p) override;
};

}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_TVM_TVMAGEGENDERIMPL_H_
