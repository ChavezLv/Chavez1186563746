/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEHAT_TVM_TVMFACEHATIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEHAT_TVM_TVMFACEHATIMPL_H_

#include "../faceHatImpl.h"
namespace arctern {
class TvmFaceHatImpl : public FaceHatImpl {
 public:
  TvmFaceHatImpl() = default;
  ~TvmFaceHatImpl() noexcept  = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEHAT_TVM_TVMFACEHATIMPL_H_
