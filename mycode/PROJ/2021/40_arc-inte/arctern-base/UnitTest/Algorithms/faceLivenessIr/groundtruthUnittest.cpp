/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "spdlog/sinks/android_sink.h"
#include <stdio.h>
#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceLivenessIr, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-liveness-ir-d-1.0.4-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-liveness-ir-d-1.0.4-i.yml";
  std::string imgDir = getDir() + "data/predict_face_liveness_ir/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-liveness-ir-d-1.0.4.bin");
  arctern::FaceLivenessIrResult result;
  FaceLivenessIrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(HISI_ALLOWED)
TEST(FaceLivenessIr, HISI) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-liveness-ir-nnie3516_1.0.5-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-liveness-ir-nnie3516_1.0.5-i.yml";
  std::string imgDir = getDir() + "data/predict_face_liveness_ir/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-liveness-ir-nnie3516_1.0.5.bin");
  FaceLivenessIrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.grayMat_ = true;
  unittest.test();
}
#elif defined(TVM_ALLOWED)
TEST(FaceLivenessIr, TVM) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-liveness-ir-f32-d-1.0.4-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/rk3288_CPU@face-liveness-ir-f32-d-1.0.4-i.yml";
  std::string imgDir = getDir() + "data/predict_face_liveness_ir/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_liveness_1.0.4/face-liveness-ir-f32-d-1.0.4.bin");
  FaceLivenessIrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(RKNN_ALLOWED)
TEST(FaceLivenessIr, RKNN105) {
  std::string ymlGroundTruthPath = getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-liveness-ir-f32-d-1.0.5-rc.yml";
  std::string ymlInputPath = getDir() +
      "input_params/face-liveness-ir-f32-d-1.0.5-i.yml";
  std::string imgDir = getDir() + "data/predict_face_liveness_ir/";
  std::string modelPath =
      std::string("/userdata/rknn/models/face-liveness-ir-rv1109N-i8-q1-d-1.0.5.bin");
  arctern::FaceLivenessIrResult result;
  FaceLivenessIrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}


#endif
