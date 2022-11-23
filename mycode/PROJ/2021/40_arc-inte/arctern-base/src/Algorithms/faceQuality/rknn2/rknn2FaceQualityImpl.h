//
// Created by gh on 2021/8/5.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEQUALITY_RKNN2_RKNN2FACEQUALITYIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEQUALITY_RKNN2_RKNN2FACEQUALITYIMPL_H_

#include "../faceQualityImpl.h"

namespace arctern {
class Rknn2FaceQualityImpl : public FaceQualityImpl {
 public:
  Rknn2FaceQualityImpl();
  ~Rknn2FaceQualityImpl();

  ErrorType Init(const InitParameter *p) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;
};
} // namespace
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEQUALITY_RKNN2_RKNN2FACEQUALITYIMPL_H_
