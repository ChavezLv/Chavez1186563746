/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.02.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEABNOMALLIGHT_TVM_TVMFACEABNOMALLIGHTIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEABNOMALLIGHT_TVM_TVMFACEABNOMALLIGHTIMPL_H_

#include "../faceAbnomalLightImpl.h"

namespace arctern {

class TvmFaceAbnomalLightImpl : public  FaceAbnomalLightImpl {
 public:
  TvmFaceAbnomalLightImpl() = default;
  ~TvmFaceAbnomalLightImpl() noexcept  = default;
  ErrorType Init(const InitParameter *p) override;
};

}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEABNOMALLIGHT_TVM_TVMFACEABNOMALLIGHTIMPL_H_