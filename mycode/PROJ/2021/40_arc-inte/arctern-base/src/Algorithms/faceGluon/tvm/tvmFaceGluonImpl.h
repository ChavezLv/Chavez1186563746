/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.30
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_BASE_TVMFACEGLUONIMPL_H
#define ARCTERN_BASE_TVMFACEGLUONIMPL_H

#include "../faceGluonImpl.h"
namespace arctern {
class TvmFaceGluonImpl : public FaceGluonImpl {
 public:
  TvmFaceGluonImpl() = default;
  ~TvmFaceGluonImpl() override = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_TVMFACEGLUONIMPL_H
