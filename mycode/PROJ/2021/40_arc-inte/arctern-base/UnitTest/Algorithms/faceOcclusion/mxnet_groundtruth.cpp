/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.18
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceOcclusion, MXNET) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-occlusion-detection-d-0.0.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-occlusion-detection-d-0.0.1-i.yml";
  std::string imgDir = getDir() + "data/predict_faceocc_attributes/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-occlusion-detection-d-0.0.1.bin");
  arctern::FaceOcclusionResult result;
  FaceOcclusionUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceOcclusion, TVM) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-occlusion-detection-d-0.0.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-occlusion-detection-d-0.0.1-i.yml";
  std::string imgDir = getDir() + "data/predict_faceocc_attributes/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-occlusion-detection-tvm-f32-d-0.0.1-var7.bin");
  arctern::FaceOcclusionResult result;
  FaceOcclusionUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif