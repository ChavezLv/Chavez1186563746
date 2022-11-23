/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FacialLandmark, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-landmark-d-1.3.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-landmark-d-1.3.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-landmark-d-1.3.0.bin");
  arctern::FacialLandmarkResult result;
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FacialLandmark, MXNETTTiny110) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-landmark-tiny-d-1.1.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-landmark-tiny-d-1.1.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark_tiny_7/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-landmark-tiny-d-1.1.0.bin");
  arctern::FacialLandmarkResult result;
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FacialLandmark, MXNETTiny120) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-landmark-tiny-d-1.2.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-landmark-tiny-d-1.2.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark_tiny_7/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-landmark-tiny-d-1.2.0.bin");
  arctern::FacialLandmarkResult result;
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(HISI_ALLOWED)
TEST(FacialLandmark, Hisi120) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-landmark-nnie3516-i8-q1-d-1.2.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-landmark-nnie3516-i8-q1-d-1.2.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-landmark-nnie3516-i8-q1-d-1.2.0.bin");
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FacialLandmark, Hisi130) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-landmark-nnie3516-i8-q1-d-1.3.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-landmark-nnie3516-i8-q1-d-1.3.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-landmark-nnie3516-i8-q1-d-1.3.0.bin");
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED)  && defined(__arm__)
TEST(FacialLandmark, Tvm130) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-landmark-f32-d-c-1.3.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/rk3288_CPU@face-landmark-f32-d-c-1.3.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_landmark_1.3.0/face-landmark-f32-d-c-1.3.0.bin");
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FacialLandmark, Tvm130) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-landmark-f32-d-c-1.3.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/rk3288_CPU@face-landmark-f32-d-c-1.3.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-landmark-tvm-f32-d-1.3.0-var7.bin");
  arctern::FacialLandmarkResult result;
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(RKNN2_ALLOWED)

TEST(FacialLandmark, RKNN2_140) {
  std::string ymlGroundTruthPath = ARCTERN_PROJECT_PATH + std::string("/yml/face-landmark-f32-d-1.4.0-rc.yml");
  std::string ymlInputPath = ARCTERN_PROJECT_PATH + std::string("/yml/face-landmark-f32-d-1.4.0-i.yml");
  std::string imgDir = ARCTERN_PROJECT_PATH + std::string("/dataset/predict_facial_landmark/");
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/model/face-landmark-f32-d-1.4.0.bin");
  arctern::FacialLandmarkResult result;
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(RKNN_ALLOWED)
TEST(FacialLandmark, RKNN130) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-landmark-d-1.3.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-landmark-d-1.3.0-i.yml";
  std::string imgDir = getDir() + "data/predict_facial_landmark/";
  std::string modelPath = std::string("/userdata/rknn/models/face-landmark-rv1109N-i8-q1-d-1.3.0.bin");
  arctern::FacialLandmarkResult result;
  FacialLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}


#endif
