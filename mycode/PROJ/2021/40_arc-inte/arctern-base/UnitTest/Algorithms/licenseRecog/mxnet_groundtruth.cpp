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
TEST(LicenseRecog, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/license-recog-lite-f32-d-0.1.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/license-recog-lite-f32-d-0.1.0-i.yml";
  std::string imgDir = getDir() + "data/license_recognition_lite/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/license-recog-lite-f32-d-0.1.0.bin");
  arctern::LicenseRecogResult result;
  LicenseRecogUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}

