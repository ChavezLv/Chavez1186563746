/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceExpression/faceExpression.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-expression-classify-d-0.0.2.bin");
#elif defined(TVM_ALLOWED)
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-expression-classify-tvm-f32-d-0.0.2-var7.bin");
#endif
}

TEST(FaceExpression, Init_NULL) {
  initNull<arctern::FaceExpression>();
}

TEST(FaceExpression, Init_WRONGParam) {
  initWrongParam<arctern::FaceExpression, arctern::InitParameter>(getModelPath());
}

TEST(FaceExpression, Init_normal) {
  initNormal<arctern::FaceExpression>(getModelPath());
}

TEST(FaceExpression, Release) {
  release<arctern::FaceExpression>();
}


TEST(FaceExpression, Process_noInit) {
  processNoInit<arctern::FaceExpression>();
}

TEST(FaceExpression, Process_Init) {
  processInitWithRect<arctern::FaceExpression, arctern::FaceExpressionRunParameter>(getModelPath());
}
}  // namespace
