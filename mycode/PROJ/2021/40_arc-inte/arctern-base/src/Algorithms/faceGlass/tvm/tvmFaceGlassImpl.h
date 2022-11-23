/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEGLASS_TVM_TVMFACEGLASSIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEGLASS_TVM_TVMFACEGLASSIMPL_H_

#include "../faceGlassImpl.h"

namespace arctern {
class TvmFaceGlassImpl : public FaceGlassImpl {
 public:
  TvmFaceGlassImpl() = default;
  ~TvmFaceGlassImpl() noexcept = default;

  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLASS_TVM_TVMFACEGLASSIMPL_H_
