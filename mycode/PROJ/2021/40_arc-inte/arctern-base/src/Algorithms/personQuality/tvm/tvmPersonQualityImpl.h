/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2021.05.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONQUALITY_TVM_TVMPERSONQUALITYIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONQUALITY_TVM_TVMPERSONQUALITYIMPL_H_

#include "../personQualityImpl.h"

namespace arctern {
class TvmPersonQualityImpl : public PersonQualityImpl {
 public:
  TvmPersonQualityImpl() = default;
  ~TvmPersonQualityImpl() noexcept = default;
  ErrorType Init(const InitParameter *p) override;
};
}


#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONQUALITY_TVM_TVMPERSONQUALITYIMPL_H_
