//
// Created by Admin on 2019/6/16.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)
TEST(DogDetTiny, TRT_video000) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/dog-det-video-f32-d-0.0.0-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_dog/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/dog-det-video-f32-d-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::dogDetTinyResult result;
  parameter.result=&result;
  TrtDogDetTinyUnittest unittest(parameter);
  unittest.test();
}
#endif