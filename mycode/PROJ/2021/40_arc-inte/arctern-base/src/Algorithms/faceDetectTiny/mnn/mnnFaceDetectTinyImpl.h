//
// Created by gh on 2021/8/26.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_MNN_MNNFACEDETECTTINYIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_MNN_MNNFACEDETECTTINYIMPL_H_

#include "../faceDetectTinyImpl.h"

namespace arctern {
class MnnFaceDetectTinyImpl : public  faceDetectTinyImpl {
 public:
  MnnFaceDetectTinyImpl() ;
  ~MnnFaceDetectTinyImpl() noexcept  = default;
  int GetModelHeight() override;
  int GetModelWidth() override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_MNN_MNNFACEDETECTTINYIMPL_H_
