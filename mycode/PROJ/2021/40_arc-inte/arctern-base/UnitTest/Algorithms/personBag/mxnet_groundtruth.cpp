/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "../../config.h"
#include "./groundtruth.h"
#include "gtest/gtest.h"
TEST(PERSONBAG, MXNET) {
  std::string ymlGroundTruthPath =
      getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/person-bag-f32-d-0.0.0-rc.yml";
  std::string ymlInputPath =
      getDir() + "input_params/person-bag-f32-d-0.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_person_bag/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/body/person-bag-f32-d-0.0.0.bin");
  personBagUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir,
                             modelPath);
  unittest.test();
}
