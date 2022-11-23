/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.03
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(PersonQuality, MXNET103) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-quality-d-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/person-quality-d-1.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_person_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-quality-d-1.0.3.bin");
  arctern::PersonQualityResult result;
  PersonQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED)
TEST(PersonQuality, TVM103) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-quality-d-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/person-quality-d-1.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_person_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-quality-tvm-f32-d-1.0.3-var7.bin");
  arctern::PersonQualityResult result;
  PersonQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif