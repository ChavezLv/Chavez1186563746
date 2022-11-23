/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.01
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "../../config.h"
#include "./groundtruth.h"
#include "gtest/gtest.h"

#ifdef MXNET_ALLOWED
TEST(FACEJOY, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH +
                          std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath =
      dir + "results_arcterncpp/LINUX_MXNET_CPU/face-joy-1.6.0-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-joy-1.6.0-i.yml";
  std::string imgDir = dir + "/data/predict_face_joy/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-joy-1.6.0.bin");
  arctern::FaceJoyResult result;
  FaceJoyUnittest ground_truth(ymlGroundTruthPath, ymlInputPath, imgDir,
                               modelPath,&result);

  ground_truth.test();
}
#endif
