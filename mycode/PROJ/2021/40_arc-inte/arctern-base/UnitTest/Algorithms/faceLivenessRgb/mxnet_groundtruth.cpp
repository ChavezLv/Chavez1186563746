/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"
TEST(FaceLivenessRgb, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-liveness-rgb-d-1.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-liveness-rgb-d-1.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_face_liveness/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-liveness-rgb-d-1.0.0.bin");
  arctern::FaceLivenessRgbResult result;
  FaceLivenessRgbUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

