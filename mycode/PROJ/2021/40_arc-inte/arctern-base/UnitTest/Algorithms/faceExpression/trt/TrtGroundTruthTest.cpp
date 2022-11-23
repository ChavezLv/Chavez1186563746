//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceExpression, TRT001) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-expression-classify-0.0.2-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-expression-classify-0.0.2-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_expression/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-expression-classify-d-0.0.2.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::FaceExpressionResult result;
  parameter.result=&result;
  TrtFaceExpressionUnittest unittest(parameter);
  unittest.test();
}

#endif