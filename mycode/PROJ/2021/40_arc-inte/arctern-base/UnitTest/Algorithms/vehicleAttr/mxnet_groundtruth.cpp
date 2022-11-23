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
TEST(VEHICLEATTR, MXNET) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/vehicle-attributes-f32-d-0.3.0-var1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/vehicle-attributes-f32-d-0.3.0-var1-i.yml";
  std::string imgDir = getDir() + "data/predict_car_attributes/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/vehicle-attributes-f32-d-0.3.0-var1.bin");
  arctern::VehicleAttrResult result;
  VehicleAttrUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

