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
TEST(PERSONPOSE, MXNET) {
  std::string ymlGroundTruthPath =
      getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/pose-predictor-f32-d-0.0.3-rc.yml";
  std::string ymlInputPath =
      getDir() + "input_params/pose-predictor-f32-d-0.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_pose/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH +
      std::string(
          "/../arctern-models/develop/body/pose-predictor-f32-d-0.0.3.bin");
  arctern::PersonPoseResult result;
  personPoseUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir,
                              modelPath,&result);
  unittest.test();
}
