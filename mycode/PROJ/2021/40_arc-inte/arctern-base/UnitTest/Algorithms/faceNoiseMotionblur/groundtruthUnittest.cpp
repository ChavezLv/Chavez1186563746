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
TEST(FaceNoiseMotionblur, MXNET009) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.0.9-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-noise-motionblur-classifier-f32-d-0.0.9-i.yml";
  std::string imgDir = getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-noise-motionblur-classifier-f32-d-0.0.9.bin");
  arctern::FaceNoiseMotionblurResult result;
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceNoiseMotionblur, MXNET008) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-d-0.0.8-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-noise-motionblur-classifier-d-0.0.8-i.yml";
  std::string imgDir =
      getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath = ARCTERN_PROJECT_PATH
      + std::string("/../arctern-models/develop/face/face-noise-motionblur-classifier-d-0.0.8.bin");
  arctern::FaceNoiseMotionblurResult result;
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceNoiseMotionblur, MXNET010) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.1.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-noise-motionblur-classifier-f32-d-0.1.0-i.yml";
  std::string imgDir = getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-noise-motionblur-classifier-f32-d-0.1.0.bin");
  arctern::FaceNoiseMotionblurResult result;
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(HISI_ALLOWED)
TEST(FaceNoiseMotionblur, HISI008) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-noise-motionblur-classifier-nnie3516-i8-q1-d-0.0.8-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-noise-motionblur-classifier-nnie3516-i8-q1-d-0.0.8-i.yml";
  std::string imgDir =
      getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath = ARCTERN_PROJECT_PATH
      + std::string("/../arctern-models/develop/face/nnie3516/face-noise-motionblur-classifier-nnie3516-i8-q1-d-0.0.8.bin");
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED) && defined(__arm__)
TEST(FaceNoiseMotionblur, TVM008) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-noise-motionblur-rk3288C-f32-d-0.0.8-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/rk3288_CPU@face-noise-motionblur-rk3288C-f32-d-0.0.8-i.yml";
  std::string imgDir =
      getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath = ARCTERN_PROJECT_PATH
      + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_noise_motionblur_0.0.8/face-noise-motionblur-rk3288C-f32-d-0.0.8.bin");
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceNoiseMotionblur, TVM009) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.0.9-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-noise-motionblur-classifier-f32-d-0.0.9-i.yml";
  std::string imgDir = getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-noise-motionblur-classifier-tvm-f32-d-0.0.9-var7.bin");
  arctern::FaceNoiseMotionblurResult result;
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceNoiseMotionblur, TVM010) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.1.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-noise-motionblur-classifier-f32-d-0.1.0-i.yml";
  std::string imgDir = getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-noise-motionblur-classifier-tvm-f32-d-0.1.0-var7.bin");
  arctern::FaceNoiseMotionblurResult result;
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(RKNN_ALLOWED)
TEST(FaceNoiseMotionblur, RKNN008) {
  std::string ymlGroundTruthPath = getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-d-0.0.8-rc.yml";
  std::string ymlInputPath = getDir() +
      "input_params/face-noise-motionblur-classifier-d-0.0.8-i.yml";
  std::string imgDir = getDir() + "data/predict_face_noise_motionblur/";
  std::string modelPath  = std::string("/userdata/rknn/models/face-noise-motionblur-classifier-rv1109N-i8-q1-d-0.0.8.bin");

  arctern::FaceNoiseMotionblurResult result;
  FaceNoiseMotionblurUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}


#endif