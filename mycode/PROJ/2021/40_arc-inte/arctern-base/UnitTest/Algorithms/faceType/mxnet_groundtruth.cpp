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
TEST(faceType, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH +
                          std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath =
      dir + "results_arcterncpp/LINUX_MXNET_CPU/face-type2-0.2.0-rc.yml";
  std::string ymlInputPath = dir + "input_params/face-type2-0.2.0-i.yml";
  std::string imgDir = dir + "/data/predict_face_type2/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-type2-0.2.0.bin");
  arctern::FaceTypeResult result;
  faceTypeUnittest ground_truth(ymlGroundTruthPath, ymlInputPath, imgDir,
                                modelPath,&result);

  ground_truth.test();
}
#endif
