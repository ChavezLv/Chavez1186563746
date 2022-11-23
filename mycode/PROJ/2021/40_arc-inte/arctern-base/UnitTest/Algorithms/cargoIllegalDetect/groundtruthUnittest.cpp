/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         author
 *  Last modified:  2021.12.21
 *  email:          nullptr
 ****************************************************************************/

#include "UnitTest/Algorithms/cargoIllegalDetect/groundtruth.h"
#include "UnitTest/config.h"
#include "gtest/gtest.h"

#ifdef MXNET_ALLOWED
TEST(CargoIllegalDetect, MXNET_video010) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/cargoIllegal-det-f32-d-0.1.0-rc.json";
std::cout << ymlPath << std:: endl;
  std::string imgDir = getDir() + "data/cargo_illegal_det_010/";
std::cout << imgDir << std:: endl;
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/cargoIllegal-det-f32-d-0.1.0.bin");
std::cout << modelPath << std:: endl;
  arctern::CargoIllegalDetectResult result;
  CargoIllegalDetectUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif
