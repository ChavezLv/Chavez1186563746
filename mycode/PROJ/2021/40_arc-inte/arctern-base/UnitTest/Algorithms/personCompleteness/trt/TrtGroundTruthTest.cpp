//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(PersonCompleteness, TRT000) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-completeness-0.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/person-completeness-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_person_completeness/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/body/person-completeness-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonCompletenessResult result;
  parameter.result=&result;
  TrtPersonCompletenessUnittest unittest(parameter);
  unittest.test();
}



#endif