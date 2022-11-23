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
TEST(VEHICLEFEAT, MXNET) {
  std::string ymlGroundTruthPath =
      getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/vehicle-feat-d-0.0.1-rc.yml";
  std::string ymlInputPath =
      getDir() + "input_params/vehicle-feat-d-0.0.1-i.yml";
  std::string imgDir = getDir() + "data/get_vehicle_feat/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH +
      std::string(
          "/../arctern-models/develop/vehicle/vehicle-feat-d-0.0.1.bin");
  VehicleFeatUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir,
                               modelPath);
  unittest.test();
}
