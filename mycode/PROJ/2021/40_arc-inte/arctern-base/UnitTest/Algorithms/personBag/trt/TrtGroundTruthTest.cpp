//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(PersonBag, TRT000) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-bag-f32-d-0.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/person-bag-f32-d-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_person_bag/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/body/person-bag-f32-d-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::PersonBagResult result;
  parameter.result=&result;
  TrtPersonBagUnittest unittest(parameter);
  unittest.test();
}


#endif