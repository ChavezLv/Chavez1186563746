/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceQuality, MXNET103) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-quality-d-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-quality-d-1.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_face_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-quality-d-1.0.3.bin");
  arctern::FaceQualityResult result;
  FaceQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceQuality, MXNET102) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-quality-d-1.0.2-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-quality-d-1.0.2-i.yml";
  std::string
      imgDir = getDir() + "data/predict_face_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-quality-d-1.0.2.bin");
  arctern::FaceQualityResult result;
  FaceQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(HISI_ALLOWED)
TEST(FaceQuality, HISI103) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-quality-nnie3516-i8-q1-d-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-quality-nnie3516-i8-q1-d-1.0.3-i.yml";
  std::string
      imgDir = getDir() + "data/predict_face_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-quality-nnie3516-i8-q1-d-1.0.3.bin");
  FaceQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED) && defined(__arm__)
TEST(FaceQuality, TVM102) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-quality-f32-d-1.0.2-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/rk3288_CPU@face-quality-f32-d-1.0.2-i.yml";
  std::string
      imgDir = getDir() + "data/predict_face_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_quality_1.0.2/face-quality-f32-d-1.0.2.bin");
  FaceQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceQuality, TVM103) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-quality-d-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-quality-d-1.0.3-i.yml";
  std::string
      imgDir = getDir() + "data/predict_face_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-quality-tvm-f32-d-1.0.3-var7.bin");
  arctern::FaceQualityResult result;
  FaceQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(RKNN_ALLOWED)
TEST(FaceQuality, RKNN103) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-quality-d-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-quality-d-1.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_face_quality/";
  std::string modelPath =
      std::string("/userdata/rknn/models/face-quality-rv1109N-i8-q1-d-0.0.3.bin");
  arctern::FaceQualityResult result;
  FaceQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#endif
