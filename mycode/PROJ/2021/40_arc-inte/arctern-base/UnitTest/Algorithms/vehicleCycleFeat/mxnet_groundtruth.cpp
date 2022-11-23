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
TEST(VEHICLECYCLEFEAT, MXNET) {
  std::string ymlGroundTruthPath =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/"
                 "vehicle-cycle-feat-f32-d-0.0.1-rc.yml";
  std::string ymlInputPath =
      getDir() + "input_params/vehicle-cycle-feat-f32-d-0.0.1-i.yml";
  std::string imgDir = getDir() + "data/get_vehicle_cycle_feat/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/"
                                         "vehicle-cycle-feat-f32-d-0.0.1.bin");
  VehicleCycleFeatUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir,
                                    modelPath);
  unittest.test();
}
