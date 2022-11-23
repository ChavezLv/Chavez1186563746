/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_BASE_TVMFACEDETECTRTNIMPL_H
#define ARCTERN_BASE_TVMFACEDETECTRTNIMPL_H

#include "../faceDetectRTNImpl.h"

namespace arctern {
class TvmFaceDetectRTNImpl : public FaceDetectRTNImpl {
 public:
  TvmFaceDetectRTNImpl() = default;
  ~TvmFaceDetectRTNImpl() override = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_TVMFACEDETECTRTNIMPL_H
