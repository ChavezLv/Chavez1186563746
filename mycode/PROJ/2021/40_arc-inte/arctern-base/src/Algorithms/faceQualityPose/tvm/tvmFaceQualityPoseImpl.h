/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.05.8
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEQUALITYPOSE_TVM_TVMFACEQUALITYPOSE_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEQUALITYPOSE_TVM_TVMFACEQUALITYPOSE_H_

#include "../faceQualityPoseImpl.h"

namespace arctern {

class TvmFaceQualityPoseImpl : public FaceQualityPoseImpl {
 public:
  TvmFaceQualityPoseImpl() = default;
  ~TvmFaceQualityPoseImpl() noexcept = default;
  ErrorType Init(const InitParameter *p) override;
};

}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEQUALITYPOSE_TVM_TVMFACEQUALITYPOSE_H_
