/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.27
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "UnitTest/config.h"
#include "UnitTest/Algorithms/multiDetectCenter/groundtruth.h"
#include "gtest/gtest.h"

TEST(MultiDetectCenter, MXNET_CENTER001) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/multi-detector-center-f32-d-0.0.1-rc.yml";
  std::string imgDir = getDir() + "data/detect_multi_cat_center/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/multi-detector-center-f32-d-0.0.1.bin");
  arctern::MultiDetectCenterResult result;
  MultiDetectCenterUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(MultiDetectCenter, BODY_FACE_HEAD100) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/body-face-head-detector-center-combined-f32-d-1.0.0-rc.json";
  std::string imgDir = getDir() + "data/body_face_head_detector_1.0/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/body-face-head-detector-center-combined-f32-d-1.0.0.bin");
  arctern::MultiDetectCenterResult result;
  MultiDetectCenterUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(MultiDetectCenter, MXNET_CENTERCOMBINED004) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/multi-detector-center-combined-f32-d-0.0.4-rc.yml";
  std::string imgDir = getDir() + "data/detect_multi_cat_center/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/multi-detector-center-combined-f32-d-0.0.4.bin");
  arctern::MultiDetectCenterResult result;
  MultiDetectCenterUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(MultiDetectCenter, MXNET_CENTERCOMBINED002) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/multi-detector-center-combined-f32-d-0.0.2-rc.yml";
  std::string imgDir = getDir() + "data/detect_multi_cat_center/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/multi-detector-center-combined-f32-d-0.0.2.bin");
  arctern::MultiDetectCenterResult result;
  MultiDetectCenterUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}


