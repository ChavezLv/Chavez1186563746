/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceHat/faceHat.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-hat-1.0.3.bin");
#elif defined(TVM_ALLOWED)
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-hat-tvm-f32-d-1.0.3-var7.bin");
#endif
}

TEST(FaceHat, Init_NULL) {
  initNull<arctern::FaceHat>();
}

TEST(FaceHat, Init_WRONGParam) {
  initWrongParam<arctern::FaceHat, arctern::InitParameter>(getModelPath());
}

TEST(FaceHat, Init_normal) {
  initNormal<arctern::FaceHat>(getModelPath());
}

TEST(FaceHat, Release) {
  release<arctern::FaceHat>();
}

TEST(FaceHat, Process_noInit) {
  processNoInit<arctern::FaceHat>();
}

TEST(FaceHat, Process_Init) {
  processInitWithRect<arctern::FaceHat, arctern::FaceHatRunParameter>(getModelPath());
}
}  // namespace
