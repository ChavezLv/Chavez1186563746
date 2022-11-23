/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceDetectRTN, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlPath = dir + "results_arcterncpp/LINUX_MXNET_CPU/face-det-retinaface-d-1.0.2-rc.yml";
  std::string imgDir = dir + "data/detect_retinaface/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-det-retinaface-d-1.0.2.bin");
  arctern::FaceDetectRTNResult result;
  FaceDetectRTNUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED) && defined(__arm__)
TEST(FaceDetectRTN, TVM_CPU011) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlPath = dir + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-det-retinaface-f32-d-0.1.1-rc.yml";
  std::string imgDir = dir + "data/detect_retinaface/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_retinaface_0.1.1/face-det-retinaface-f32-d-0.1.1.bin");
  FaceDetectRTNUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceDetectRTN, TVM_GPU011) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlPath = dir + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_GPU@face-det-retinaface-f32-d-0.1.1-rc.yml";
  std::string imgDir = dir + "data/detect_retinaface/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_GPU_retinaface_f32_0.1.1/face-det-retinaface-f32-d-0.1.1.bin");
  FaceDetectRTNUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.deviceType_ = arctern::ARCTERN_OPENCL;
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceDetectRTN, TVM_CPU012) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlPath = dir + "results_arcterncpp/LINUX_MXNET_CPU/face-det-retinaface-d-1.0.2-rc.yml";
  std::string imgDir = dir + "data/detect_retinaface/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-retinaface-tvm-f32-d-1.0.2-var7.bin");
  arctern::FaceDetectRTNResult result;
  FaceDetectRTNUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

#endif
