/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.03.02
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/


#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONFEAT_TVM_TVMPERSONFEATIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONFEAT_TVM_TVMPERSONFEATIMPL_H_

#include "../personFeatImpl.h"

namespace arctern {
class TvmPersonFeatImpl : public PersonFeatImpl {
 public:
  TvmPersonFeatImpl() = default;
  ~TvmPersonFeatImpl() noexcept  = default;
  ErrorType Init(const InitParameter *p) override;
};
}


#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONFEAT_TVM_TVMPERSONFEATIMPL_H_
