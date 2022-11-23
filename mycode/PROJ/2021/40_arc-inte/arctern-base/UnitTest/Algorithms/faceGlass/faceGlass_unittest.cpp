/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceGlass/faceGlass.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-glass-1.0.3.bin");
#elif defined(TVM_ALLOWED)
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-glass-tvm-f32-d-1.0.3-var7.bin");
#endif
}

TEST(FaceGlass, Init_NULL) {
  initNull<arctern::FaceGlass>();
}

TEST(FaceGlass, Init_WRONGParam) {
  initWrongParam<arctern::FaceGlass, arctern::InitParameter>(getModelPath());
}

TEST(FaceGlass, Init_normal) {
  initNormal<arctern::FaceGlass>(getModelPath());
}

TEST(FaceGlass, Release) {
  release<arctern::FaceGlass>();
}


TEST(FaceGlass, Process_noInit) {
  processNoInit<arctern::FaceGlass>();
}

TEST(FaceGlass, Process_Init) {
  processInitWithRect<arctern::FaceGlass, arctern::FaceGlassRunParameter>(getModelPath());
}
}  // namespace
