/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.07
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "UnitTest/Algorithms/dogDetTiny/groundtruth.h"
#include "UnitTest/config.h"
#include "gtest/gtest.h"

#ifdef MXNET_ALLOWED
TEST(dogDetectTiny, MXNET_tiny200) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/dog-det-video-f32-d-0.0.0-rc.yml";
  std::string imgDir = getDir() + "data/detect_dog/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/unclassified/dog-det-video-f32-d-0.0.0.bin");

  arctern::dogDetTinyResult result;
  dogDetTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif
