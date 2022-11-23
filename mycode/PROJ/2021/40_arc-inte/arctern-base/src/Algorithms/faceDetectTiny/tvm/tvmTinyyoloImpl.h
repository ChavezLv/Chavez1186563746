/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.3.4
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_TVM_TVMTINYYOLOIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_TVM_TVMTINYYOLOIMPL_H_

#include "../faceDetectTinyImpl.h"

namespace arctern {
class TvmYoloV3 : public  faceDetectTinyImpl {
 public:
  TvmYoloV3() = default;
  ~TvmYoloV3() noexcept  = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_TVM_TVMTINYYOLOIMPL_H_
