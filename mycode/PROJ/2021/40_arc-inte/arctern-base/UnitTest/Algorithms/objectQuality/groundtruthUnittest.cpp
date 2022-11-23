/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.16
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

TEST(ObjectQuality, MXNET_CAR000) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/car-quality-d-0.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/car-quality-d-0.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_car_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/car-quality-d-0.0.0.bin");
  arctern::ObjectQualityResult result;
  ObjectQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(ObjectQuality, MXNET_EBIKE000) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/ebike-quality-d-0.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/ebike-quality-d-0.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_ebike_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/ebike-quality-d-0.0.0.bin");
  arctern::ObjectQualityResult result;
  ObjectQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(ObjectQuality, MXNET_LICENSE000) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/license-quality-d-0.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/license-quality-d-0.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_license_quality/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/license-quality-d-0.0.0.bin");
  arctern::ObjectQualityResult result;
  ObjectQualityUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
