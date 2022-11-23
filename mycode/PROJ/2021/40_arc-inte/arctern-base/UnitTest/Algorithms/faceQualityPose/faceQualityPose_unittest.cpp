/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceQualityPose/faceQualityPose.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::FaceQualityPose;
using arctern::FaceQualityPoseRunParameter;

namespace {
std::string getModelPath() {
#ifdef MXNET_ALLOWED
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-quality-pose-0.0.1.bin");
#elif defined(TVM_ALLOWED)
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-quality-pose-tvm-f32-d-0.0.1-var7.bin");
#endif
  return modelPath;
}

TEST(FaceQualityPose, Init_NULL) {
  initNull<arctern::FaceQualityPose>();
}

TEST(FaceQualityPose, Init_WRONGParam) {
  initWrongParam<arctern::FaceQualityPose, arctern::InitParameter>(getModelPath());
}

TEST(FaceQualityPose, Init_normal) {
  initNormal<arctern::FaceQualityPose>(getModelPath());
}

TEST(FaceQualityPose, Release) {
  release<arctern::FaceQualityPose>();
}

TEST(FaceQualityPose, Process_noInit) {
  processNoInit<arctern::FaceQualityPose>();
}

TEST(FaceQualityPose, Process_Init) {
  processInitWithRect<arctern::FaceQualityPose, arctern::FaceQualityPoseRunParameter>(getModelPath());
}
}  // namespace
