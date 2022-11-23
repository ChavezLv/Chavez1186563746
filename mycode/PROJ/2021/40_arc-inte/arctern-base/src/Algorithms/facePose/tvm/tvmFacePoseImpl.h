/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEPOSE_TVM_TVMFACEPOSEIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEPOSE_TVM_TVMFACEPOSEIMPL_H_

#include "../facePoseImpl.h"

namespace arctern {
class TvmFacePoseImpl : public FacePoseImpl {
 public:
  TvmFacePoseImpl() = default;
  ~TvmFacePoseImpl() override  = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEPOSE_TVM_TVMFACEPOSEIMPL_H_
