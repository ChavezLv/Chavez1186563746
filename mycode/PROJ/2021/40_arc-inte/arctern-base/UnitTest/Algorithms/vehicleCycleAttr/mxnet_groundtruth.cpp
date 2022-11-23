/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"
TEST(VEHICLECYCLEATTR, MXNET) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/vehicle-cycle-attributes-f32-d-0.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/vehicle-cycle-attributes-f32-d-0.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_vehicle_cycle_attributes/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/vehicle-cycle-attributes-f32-d-0.0.3.bin");
  arctern::VehicleCycleAttrResult result;
  VehicleCycleAttrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

