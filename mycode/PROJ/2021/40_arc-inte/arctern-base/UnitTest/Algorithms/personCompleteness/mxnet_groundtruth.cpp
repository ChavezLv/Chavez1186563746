/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.07
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(PersonCompleteness, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-completeness-0.0.0-rc.yml";
  //std::string ymlGroundTruthPath = "./person-completeness-0.0.0-rc.yml";
  //std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-1.9.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/person-completeness-0.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_person_completeness/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-completeness-0.0.0.bin");
  arctern::PersonCompletenessResult result;
  PersonCompletenessUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}
#elif  TVM_ALLOWED
TEST(PersonCompleteness, TVM000) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-completeness-0.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/person-completeness-0.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_person_completeness/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-completeness-tvm-f32-d-0.0.0-var7.bin");
  arctern::PersonCompletenessResult result;
  PersonCompletenessUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}
#endif


