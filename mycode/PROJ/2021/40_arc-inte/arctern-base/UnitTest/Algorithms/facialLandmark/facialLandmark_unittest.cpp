/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "src/Algorithms/facialLandmark/facialLandmark.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-landmark-d-1.3.0.bin");
#elif defined(HISI_ALLOWED)
  return ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/nnie3516/face-landmark-nnie3516-i8-q1-d-1.2.0.bin");
#elif defined(TVM_ALLOWED) && defined(__arm__)
  return ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_landmark_1.3.0/face-landmark-f32-d-c-1.3.0.bin");
#elif defined(TVM_ALLOWED)
  return ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-landmark-tvm-f32-d-1.3.0-var7.bin");
#elif defined(RKNN2_ALLOWED)
  return ARCTERN_PROJECT_PATH +
      std::string("/model/face-landmark-f32-d-1.4.0.bin");
#endif
}

TEST(FacialLandmark, Init_NULL) {
  initNull<arctern::FacialLandmark>();
}

TEST(FacialLandmark, Init_WRONGParam) {
  initWrongParam<arctern::FacialLandmark, arctern::InitParameter>(getModelPath());
}

TEST(FacialLandmark, Init_normal) {
  initNormal<arctern::FacialLandmark>(getModelPath());
}

TEST(FacialLandmark, Release) {
  release<arctern::FacialLandmark>();
}

TEST(FacialLandmark, Process_noInit) {
  processNoInit<arctern::FacialLandmark>();
}

TEST(FacialLandmark, Process_Init) {
  processInitWithRect<arctern::FacialLandmark, arctern::FacialLandmarkRunParameter>(getModelPath());
}
}  // namespace
