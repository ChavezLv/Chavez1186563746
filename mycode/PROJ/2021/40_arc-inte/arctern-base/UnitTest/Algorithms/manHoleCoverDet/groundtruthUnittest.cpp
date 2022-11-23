/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         admin
 *  Last modified:  2021.12.20
 ****************************************************************************/

#include "UnitTest/config.h"
#include "UnitTest/Algorithms/manHoleCoverDet/groundtruth.h"
#include "gtest/gtest.h"

/* #ifdef MXNET_ALLOWED */
TEST(FaceDetectTiny, MXNET_video010) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/manholeCover-abnormal-det-f32-d-0.2.0-rc.json";
  std::string imgDir = getDir() + "data/manholeCover_abnormal_det_020/";
  std::string modelPath = ARCTERN_PROJECT_PATH + \
      std::string("/../arctern-models/develop/unclassified/manholeCover-abnormal-det-f32-d-0.2.0.bin");
  arctern::ManHoleCoverDetResult result;
  ManHoleCoverDetUnittest unittest(ymlPath, imgDir, modelPath, &result);
  unittest.test();
}
/* #endif */
