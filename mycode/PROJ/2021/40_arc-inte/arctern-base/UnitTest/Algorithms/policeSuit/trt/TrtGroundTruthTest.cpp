//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "UnitTest/config.h"
#include "gtest/gtest.h"
#include <utility>
#if defined(TRT_ALLOWED)

TEST(PoliceSuit, TRT001) {
  TrtPoliceSuitTestParam parameter;
  parameter.ymlGroundTruth =
      getDir() + "results_arcterncpp/LINUX_MXNET_CPU/"
                 "police-suit-predictor-f32-d-0.0.0-rc.yml";
  parameter.ymlInput =
      getDir() + "input_params/police-suit-predictor-f32-d-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_police_suit/";
  parameter.modelPath =
      ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/"
                                    "police-suit-predictor-f32-d-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PoliceSuitResult result;
  parameter.result=&result;
  TrtPoliceSuitUnittest unittest(parameter);
  unittest.test();
}
#endif
