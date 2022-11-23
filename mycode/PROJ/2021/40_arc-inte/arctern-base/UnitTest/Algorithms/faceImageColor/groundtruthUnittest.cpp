/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"
#ifdef MXNET_ALLOWED
TEST(FaceImageColor, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/LINUX_MXNET_CPU/face-mask-d-0.2.1-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-mask-d-0.2.1-i.yml";
  std::string imgDir = dir + "data/predict_face_mask/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-mask-d-0.2.1.bin");
  FaceImageColorUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(HISI_ALLOWED)
TEST(FaceImageColor, HISI) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/ARMLINUX_NNIE3516/face-image-color-nnie-1.0.1-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-image-color-nnie-1.0.1-i.yml";
  std::string imgDir = dir + "data/predict_face_image_color/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-image-color-nnie-1.0.1.bin");
  FaceImageColorUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED)
TEST(FaceImageColor, TVM) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-image-color-f32-d-1.0.2-rc.yml";
  std::string ymlInputPath = dir + "input_params/rk3288_CPU@face-image-color-f32-d-1.0.2-i.yml";
  std::string imgDir = dir + "data/predict_face_image_color/";
  std::string modelPath = ARCTERN_PROJECT_PATH +   std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_image_color_1.0.2/face-image-color-f32-d-1.0.2.bin");
  FaceImageColorUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(RKNN_ALLOWED)
TEST(FaceImageColor, RKNN101) {
  std::string ymlGroundTruthPath = getDir() +
      "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-image-color-f32-d-1.0.2-rc.yml";
  std::string ymlInputPath = getDir() +
      "input_params/rk3288_CPU@face-image-color-f32-d-1.0.2-i.yml";
  std::string imgDir = getDir() + "data/predict_face_image_color/";
  std::string modelPath =
      std::string("/userdata/rknn/models/face-image-color-rv1109N-i8-q1-d-1.0.1.bin");

  arctern::FaceImageColorResult result;
    FaceImageColorUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
    unittest.test();
}

#endif