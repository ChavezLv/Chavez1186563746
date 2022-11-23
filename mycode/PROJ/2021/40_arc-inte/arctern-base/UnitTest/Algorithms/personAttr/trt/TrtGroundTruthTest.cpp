//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(PersonAttr, TRT020) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-attr-d-0.2.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/person-attr-d-0.2.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_person_attributes/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/body/person-attr-d-0.2.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonAttrResult result;
  parameter.result=&result;
  TrtPersonAttrUnittest unittest(parameter);
  unittest.test();
}



#endif