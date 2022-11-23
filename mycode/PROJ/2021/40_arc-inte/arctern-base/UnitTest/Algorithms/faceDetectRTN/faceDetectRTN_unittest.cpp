/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceDetectRTN/faceDetectRTN.h"
#include "UnitTest/base-unittest.h"
#include "../../config.h"

namespace {
std::string getModelPath() {
#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH
      + std::string("/../arctern-models/develop/face/face-det-retinaface-d-1.0.2.bin");
#elif defined(TVM_ALLOWED) && defined(__arm__)
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_liveness_1.0.4/face-liveness-ir-f32-d-1.0.4.bin");
#elif defined(TVM_ALLOWED)
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-retinaface-tvm-f32-d-1.0.2-var7.bin");
#endif
}

TEST(FaceDetectRTN, Init_NULL) {
  initNull<arctern::FaceDetectRTN>();
}

TEST(FaceDetectRTN, Init_WRONGParam) {
  initWrongParam<arctern::FaceDetectRTN, arctern::InitParameter>(getModelPath());
}

TEST(FaceDetectRTN, Init_normal) {
  initNormal<arctern::FaceDetectRTN>(getModelPath());
}

TEST(FaceDetectRTN, Release) {
  release<arctern::FaceDetectRTN>();
}


TEST(FaceDetectRTN, Parameter_Init) {
  arctern::FaceDetectRTN det;

  /// init
  ::arctern::InitParameter initParam;
  initParam.model_path = getModelPath();
  initParam.batchNum_ = 2;
  initParam.thresh_ = 0.5;
  EXPECT_EQ(det.Init(&initParam), arctern::ErrorType::ERR_SUCCESS);
}

TEST(FaceDetectRTN, Process_noInit) {
  processNoInit<arctern::FaceDetectRTN>();
}

TEST(FaceDetectRTN, Process_Init) {
  processInitCommon<arctern::FaceDetectRTN, arctern::RunParameter>(getModelPath());
}
}  // namespace
