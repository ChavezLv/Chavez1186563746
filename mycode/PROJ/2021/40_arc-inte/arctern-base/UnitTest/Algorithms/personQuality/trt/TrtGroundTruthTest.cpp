//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(PersonQuality, TRT103) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-quality-d-1.0.3-rc.yml";
  parameter.ymlInput = getDir() + "input_params/person-quality-d-1.0.3-i.yml";
  parameter.imgPath = getDir() + "data/predict_person_quality/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/body/person-quality-d-1.0.3.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonQualityResult result;
  parameter.result=&result;
  TrtPersonQualityUnittest unittest(parameter);
  unittest.test();
}



#endif