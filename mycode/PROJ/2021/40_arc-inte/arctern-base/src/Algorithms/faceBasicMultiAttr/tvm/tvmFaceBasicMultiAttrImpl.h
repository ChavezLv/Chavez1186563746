/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.03.02
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEBASICMULTIATTR_TVM_TVMFACEBASICMULTIATTRIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEBASICMULTIATTR_TVM_TVMFACEBASICMULTIATTRIMPL_H_

#include "../faceBasicMultiAttrImpl.h"
namespace arctern {
class TvmFaceBasicMultiAttrImpl : public FaceBasicMultiAttrImpl {
 public:
  TvmFaceBasicMultiAttrImpl() = default;
  ~TvmFaceBasicMultiAttrImpl() noexcept = default;
  ErrorType Init(const InitParameter *p) override;
};

}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEBASICMULTIATTR_TVM_TVMFACEBASICMULTIATTRIMPL_H_
