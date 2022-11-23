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
TEST(FaceMask, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/LINUX_MXNET_CPU/face-mask-d-0.2.1-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-mask-d-0.2.1-i.yml";
  std::string imgDir = dir + "data/predict_face_mask/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-mask-d-0.2.1.bin");
  arctern::FaceMaskResult result;
  FaceMaskUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(HISI_ALLOWED)
TEST(FaceMask, HISI) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/ARMLINUX_NNIE3516/face-mask-nnie3516-i8-d-0.2.1-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-mask-nnie3516-i8-d-0.2.1-i.yml";
  std::string imgDir = dir + "data/predict_face_mask/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-mask-nnie3516-i8-d-0.2.1.bin");
  FaceMaskUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED) && defined(__arm__)
TEST(FaceMask, TVM) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-mask-f32-d-0.2.1-rc.yml";
  std::string ymlInputPath = dir + "input_params/rk3288_CPU@face-mask-f32-d-0.2.1-i.yml";
  std::string imgDir = dir + "data/predict_face_mask/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_mask_0.2.1/face-mask-f32-d-0.2.1.bin");
  FaceMaskUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED)
TEST(FaceMask, TVM) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-mask-f32-d-0.2.1-rc.yml";
  std::string ymlInputPath = dir + "input_params/rk3288_CPU@face-mask-f32-d-0.2.1-i.yml";
  std::string imgDir = dir + "data/predict_face_mask/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-mask-tvm-f32-d-0.2.1-var7.bin");
   arctern::FaceMaskResult result;
  FaceMaskUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(RKNN_ALLOWED)
TEST(FaceMask, RKNN021) {
  std::string ymlGroundTruthPath = getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-mask-d-0.2.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-mask-d-0.2.1-i.yml";
  std::string imgDir = getDir() + "data/predict_face_mask/";
  std::string modelPath =
      std::string("/userdata/rknn/models/face-mask-rv1109N-i8-q1-d-0.2.2.bin");
  arctern::FaceMaskResult result;
  FaceMaskUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif