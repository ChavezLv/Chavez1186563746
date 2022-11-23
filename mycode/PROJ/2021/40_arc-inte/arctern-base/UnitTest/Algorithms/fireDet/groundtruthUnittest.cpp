//
// Created by Admin on 2021/12/22.
//

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FireDet, MXNET_FIRE_010) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/fire-det-f32-d-0.1.0-rc.json";
  std::string ymlInputPath = "";
  std::string imgDir = getDir() + "data/fire_det_010/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/fire-det-f32-d-0.1.0.bin");
  arctern::FireDetResult result;
  FireDetUnittest unittest(ymlGroundTruthPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FireDet, MXNET_SMOKE_010) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/smoke-det-f32-d-0.1.0-rc.json";
  std::string ymlInputPath = "";
  std::string imgDir = getDir() + "data/smoke_det_010/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/smoke-det-f32-d-0.1.0.bin");
  arctern::FireDetResult result;
  FireDetUnittest unittest(ymlGroundTruthPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FireDet, MXNET_FIRE_SMOKE_020) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/fireSmoke-det-f32-d-0.2.0-rc.json";
  std::string ymlInputPath = "";
  std::string imgDir = getDir() + "data/fire_smoke_020/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/fireSmoke-det-f32-d-0.2.0.bin");
  arctern::FireDetResult result;
  FireDetUnittest unittest(ymlGroundTruthPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif