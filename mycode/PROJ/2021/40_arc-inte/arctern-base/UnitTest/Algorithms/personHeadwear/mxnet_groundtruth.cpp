/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.02
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"
TEST(PersonHeadwear, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-headwear-f32-d-0.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/person-headwear-f32-d-0.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_person_headwear/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-headwear-f32-d-0.0.0.bin");
  arctern::PersonHeadwearResult result;
  PersonHeadwearUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}

