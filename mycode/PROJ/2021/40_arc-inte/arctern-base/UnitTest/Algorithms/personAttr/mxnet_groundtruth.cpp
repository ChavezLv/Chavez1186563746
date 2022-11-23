/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"
TEST(PERSONATTR, MXNET) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-attr-d-0.2.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/person-attr-d-0.2.0-i.yml";
  std::string imgDir = getDir() + "data/predict_person_attributes/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-attr-d-0.2.0.bin");
  arctern::PersonAttrResult result;
  PersonAttrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

