/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.17
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "../../config.h"
#include "./groundtruth.h"
#include "gtest/gtest.h"

#ifdef MXNET_ALLOWED
TEST(PoliceSuit, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH +
                          std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath =
      dir + "results_arcterncpp/LINUX_MXNET_CPU/"
            "police-suit-predictor-f32-d-0.0.0-rc.yml";
  std::string ymlInputPath =
      dir + "input_params/police-suit-predictor-f32-d-0.0.0-i.yml";
  std::string imgDir = dir + "/data/predict_police_suit/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
                          std::string("/../arctern-models/develop/face/"
                                      "police-suit-predictor-f32-d-0.0.0.bin");
  PoliceSuitUnittest ground_truth(ymlGroundTruthPath, ymlInputPath, imgDir,
                                  modelPath);

  ground_truth.test();
}
#endif
