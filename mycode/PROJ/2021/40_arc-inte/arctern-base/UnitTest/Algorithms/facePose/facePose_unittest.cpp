/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/facePose/facePose.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::FacePose;
using arctern::FacePoseRunParameter;

namespace {
std::string getModelPath() {
#ifdef MXNET_ALLOWED
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-pose-cls-d-1.0.0.bin");
#elif defined(TVM_ALLOWED)
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-pose-cls-tvm-f32-d-1.0.0-var7.bin");
#endif
  return modelPath;
}

TEST(FacePose, Init_NULL) {
  initNull<arctern::FacePose>();
}

TEST(FacePose, Init_WRONGParam) {
  initWrongParam<arctern::FacePose, arctern::InitParameter>(getModelPath());
}

TEST(FacePose, Init_normal) {
  initNormal<arctern::FacePose>(getModelPath());
}

TEST(FacePose, Release) {
  release<arctern::FacePose>();
}

TEST(FacePose, Process_noInit) {
  processNoInit<arctern::FacePose>();
}

TEST(FacePose, Process_Init) {
  processInitWithRect<arctern::FacePose, arctern::FacePoseRunParameter>(getModelPath());
}
}  // namespace
