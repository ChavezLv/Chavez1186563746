//
// Created by Admin on 2019/6/15.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(PersonFeat, TRT_TINY050) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/person-feat-tiny-f32-d-0.5.0-i.yml";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-tiny-f32-d-0.5.0-rc.yml";
  parameter.imgPath = getDir() + "data/get_pedestrian_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-feat-tiny-f32-d-0.5.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonFeatResult result;
  parameter.result=&result;
  TrtPersonFeatUnittest unittest(parameter);
  unittest.test();
}

/*TEST(PersonFeat, TRT_100) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/person-feat-f32-d-1.0.0-i.yml";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-f32-d-1.0.0-rc.yml";
  parameter.imgPath = getDir() + "data/get_pedestrian_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-feat-f32-d-1.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonFeatResult result;
  parameter.result=&result;

  TrtPersonFeatUnittest unittest(parameter);
  unittest.test();
}
*/
TEST(PersonFeat, TRT_090) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/person-feat-f32-d-0.9.0-i.yml";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-f32-d-0.9.0-rc.yml";
  parameter.imgPath = getDir() + "data/get_pedestrian_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-feat-f32-d-0.9.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonFeatResult result;
  parameter.result=&result;

  TrtPersonFeatUnittest unittest(parameter);
  unittest.test();
}
#endif