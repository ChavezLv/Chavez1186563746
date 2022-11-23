

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"
TEST(LicenseLandmark, MXNET100) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/license-landmark-f32-d-1.0.0-rc.json";
  std::string ymlInputPath = "";
  std::string imgDir = getDir() + "data/licenseRecog/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/license-landmark-d-1.0.0.bin");
  arctern::LicenseLandmarkResult result;
  LicenseLandmarkUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath, &result);
  unittest.test();
}

