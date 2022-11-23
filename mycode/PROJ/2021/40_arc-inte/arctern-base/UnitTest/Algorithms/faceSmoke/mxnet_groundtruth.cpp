/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"
TEST(FaceSmoke, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/smoke-d-0.0.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/smoke-d-0.0.1-i.yml";
  std::string imgDir = getDir() + "data/predict_smoke/";
  std::string modelPath = ARCTERN_PROJECT_PATH +std::string("/../arctern-models/develop/face/smoke-d-0.0.1.bin");
  arctern::FaceSmokeResult result;
  FaceSmokeUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

