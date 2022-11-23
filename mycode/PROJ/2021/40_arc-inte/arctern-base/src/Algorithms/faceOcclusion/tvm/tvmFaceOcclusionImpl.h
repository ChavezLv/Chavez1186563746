/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEOCCLUSION_TVM_TVMFACEOCCLUSIONIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEOCCLUSION_TVM_TVMFACEOCCLUSIONIMPL_H_

#include "../faceOcclusionImpl.h"

namespace arctern {
class TvmFaceOcclusionImpl : public FaceOcclusionImpl {
 public:
  TvmFaceOcclusionImpl() = default;
  ~TvmFaceOcclusionImpl() noexcept  = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEOCCLUSION_TVM_TVMFACEOCCLUSIONIMPL_H_
