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
TEST(AGEGender, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/LINUX_MXNET_CPU/face-gender-age-d-0.1.0-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-gender-age-d-0.1.0-i.yml";
  std::string imgDir = dir + "data/predict_face_gender_age/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-gender-age-d-0.1.0.bin");
  arctern::AgeGenderResult result;
  AgeGenderUnittest ground_truth(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  ground_truth.test();
}

#elif defined(TVM_ALLOWED)

TEST(AGEGender, TVM010) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/LINUX_MXNET_CPU/face-gender-age-d-0.1.0-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-gender-age-d-0.1.0-i.yml";
  std::string imgDir = dir + "data/predict_face_gender_age/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gender-age-tvm-f32-d-0.1.0-var7.bin");
  arctern::AgeGenderResult result;
  AgeGenderUnittest ground_truth(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  ground_truth.test();
}

TEST(AGEGender, TVM020) {
  const std::string dir = ARCTERN_PROJECT_PATH + std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + "results_arcterncpp/LINUX_MXNET_CPU/face-gender-age-f32-d-0.2.0-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-gender-age-f32-d-0.2.0-i.yml";
  std::string imgDir = dir + "data/predict_face_gender_age/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gender-age-tvm-f32-d-0.2.0-var7.bin");
  arctern::AgeGenderResult result;
  AgeGenderUnittest ground_truth(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  ground_truth.test();
}
#endif