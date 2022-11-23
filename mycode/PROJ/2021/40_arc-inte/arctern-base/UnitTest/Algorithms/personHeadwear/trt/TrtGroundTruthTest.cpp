//
// Created by Admin on 2019/6/15.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)

TEST(PersonHeadWear, TRT) {
  TrtTestParam parameter;
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonHeadwearResult result;
  parameter.result=&result;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-headwear-f32-d-0.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/person-headwear-f32-d-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_person_headwear/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-headwear-f32-d-0.0.0.bin");

  TrtPersonHeadwearUnittest unittest(parameter);
  unittest.test();
}

#endif