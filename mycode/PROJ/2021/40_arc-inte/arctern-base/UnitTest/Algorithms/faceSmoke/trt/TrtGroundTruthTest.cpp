//
// Created by Admin on 2019/6/15.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)

TEST(FaceSmoke, TRT_001) {
  TrtTestParam parameter;
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceSmokeResult result;
  parameter.result=&result;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/smoke-d-0.0.1-rc.yml";
  parameter.ymlInput = getDir() + "input_params/smoke-d-0.0.1-i.yml";
  parameter.imgPath = getDir() + "data/predict_smoke/";
  parameter.modelPath = ARCTERN_PROJECT_PATH +std::string("/../arctern-models/develop/face/smoke-d-0.0.1.bin");
  TrtFaceSmokeUnittest unittest(parameter);
  unittest.test();
}

#endif