/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceLivenessIr/faceLivenessIr.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::FaceLivenessIr;
using arctern::FaceLivenessIrRunParameter;

namespace {
std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH;

#ifdef MXNET_ALLOWED
  modelPath += std::string("/../arctern-models/develop/face/face-liveness-ir-d-1.0.4.bin");
#elif defined(HISI_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/nnie3516/face-liveness-ir-nnie3516_1.0.5.bin");
#elif defined(TVM_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_liveness_1.0.4/face-liveness-ir-f32-d-1.0.4.bin");
#elif defined(RKNN_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/rknn/face-liveness-ir-1.0.5.rknn");
#endif

  return modelPath;
}

TEST(FaceLivenessIr, Init_NULL) {
  initNull<arctern::FaceLivenessIr>();
}

TEST(FaceLivenessIr, Init_WRONGParam) {
  initWrongParam<arctern::FaceLivenessIr, arctern::InitParameter>(getModelPath());
}

TEST(FaceLivenessIr, Init_normal) {
  initNormal<arctern::FaceLivenessIr>(getModelPath());
}

TEST(FaceLivenessIr, Release) {
  release<arctern::FaceLivenessIr>();
}

TEST(FaceLivenessIr, Process_noInit) {
  processNoInit<arctern::FaceLivenessIr>();
}

TEST(FaceLivenessIr, Process_Init) {
  processInitWithRect<arctern::FaceLivenessIr, arctern::FaceLivenessIrRunParameter>(getModelPath());
}
}  // namespace
