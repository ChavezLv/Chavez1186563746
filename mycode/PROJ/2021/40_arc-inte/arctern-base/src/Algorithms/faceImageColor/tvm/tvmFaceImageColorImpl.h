/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.26
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_TVMFACEIMAGECOLORIMPL_H
#define ARCTERN_BASE_TVMFACEIMAGECOLORIMPL_H

#include "../faceImageColorImpl.h"

namespace arctern {
class TvmFaceImageColorImpl : public FaceImageColorImpl {
 public:
  TvmFaceImageColorImpl() = default;
  ~TvmFaceImageColorImpl() override = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_TVMFACEIMAGECOLORIMPL_H
