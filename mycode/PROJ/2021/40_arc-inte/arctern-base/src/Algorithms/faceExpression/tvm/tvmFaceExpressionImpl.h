/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEEXPRESSION_TVM_TVMFACEEXPRESSIONIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEEXPRESSION_TVM_TVMFACEEXPRESSIONIMPL_H_

#include "../faceExpressionImpl.h"

namespace arctern {
class TvmFaceExpressionImpl : public FaceExpressionImpl {
 public:
  TvmFaceExpressionImpl() = default;
  ~TvmFaceExpressionImpl() noexcept  = default;

  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEEXPRESSION_TVM_TVMFACEEXPRESSIONIMPL_H_
