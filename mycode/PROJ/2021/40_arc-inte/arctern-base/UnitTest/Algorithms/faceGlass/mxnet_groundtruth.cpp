/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceGlas, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-glass-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-glass-1.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_face_glass/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-glass-1.0.3.bin");
  arctern::FaceGlassResult result;
  FaceGlassUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceGlas, TVM) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-glass-1.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-glass-1.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_face_glass/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-glass-tvm-f32-d-1.0.3-var7.bin");
  arctern::FaceGlassResult result;
  FaceGlassUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif