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
TEST(FaceIdentify, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH +
                          std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath =
      dir + "results_arcterncpp/LINUX_MXNET_CPU/face-identity-d-0.0.1-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-identity-d-0.0.1-i.yml";
  std::string imgDir = dir + "/data/predict_face_identity/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-identity-d-0.0.1.bin");
  FaceIdentifyUnittest ground_truth(ymlGroundTruthPath, ymlInputPath, imgDir,
                                modelPath);

  ground_truth.test();
}
#endif
