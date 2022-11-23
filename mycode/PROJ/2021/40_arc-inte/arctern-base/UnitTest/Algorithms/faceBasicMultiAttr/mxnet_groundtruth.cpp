/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.25
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FACEBASICMULTIATTR, MXNET) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-multi-attributes-f32-d-0.0.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-multi-attributes-f32-d-0.0.1-i.yml";
  std::string imgDir = getDir() + "data/predict_facebasic_multiattributes/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-multi-attributes-f32-d-0.0.1.bin");
  arctern::FaceBasicMultiAttrResult result;
  FaceBasicMultiAttrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FACEBASICMULTIATTR, MXNET011) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-multi-attributes-f32-d-0.1.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-multi-attributes-f32-d-0.1.1-i.yml";
  std::string imgDir = getDir() + "data/predict_facebasic_multiattributes/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-multi-attributes-f32-d-0.1.1.bin");
  arctern::FaceBasicMultiAttrResult result;
  FaceBasicMultiAttrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif TVM_ALLOWED

TEST(FACEBASICMULTIATTR, TVM011) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-multi-attributes-f32-d-0.1.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-multi-attributes-f32-d-0.1.1-i.yml";
  std::string imgDir = getDir() + "data/predict_facebasic_multiattributes/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-multi-attributes-tvm-f32-d-0.1.1-var7.bin");
  arctern::FaceBasicMultiAttrResult result;
  FaceBasicMultiAttrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#endif