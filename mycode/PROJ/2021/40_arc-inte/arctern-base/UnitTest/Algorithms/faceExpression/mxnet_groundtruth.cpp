/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceExpression, MXNET) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-expression-classify-0.0.2-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-expression-classify-0.0.2-i.yml";
  std::string imgDir = getDir() + "data/predict_face_expression/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-expression-classify-d-0.0.2.bin");
  arctern::FaceExpressionResult result;
  FaceExpressionUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceExpression, TVM) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-expression-classify-0.0.2-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-expression-classify-0.0.2-i.yml";
  std::string imgDir = getDir() + "data/predict_face_expression/";
  std::string
      modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-expression-classify-tvm-f32-d-0.0.2-var7.bin");
  arctern::FaceExpressionResult result;
  FaceExpressionUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif