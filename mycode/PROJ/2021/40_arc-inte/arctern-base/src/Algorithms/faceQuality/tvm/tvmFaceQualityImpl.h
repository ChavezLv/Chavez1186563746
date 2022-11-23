/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/


#ifndef ARCTERN_BASE_TVMFACEQUALITYIMPL_H
#define ARCTERN_BASE_TVMFACEQUALITYIMPL_H


#include "../faceQualityImpl.h"

namespace arctern {
class TvmFaceQualityImpl : public FaceQualityImpl {
 public:
  TvmFaceQualityImpl() = default;
  ~TvmFaceQualityImpl() override = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_TVMFACEQUALITYIMPL_H
