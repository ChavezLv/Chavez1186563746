//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#if defined(TRT_ALLOWED)


TEST(FaceBangs, TRT000) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-bangs-f32-d-0.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-bangs-f32-d-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_bangs/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-bangs-f32-d-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceBangsResult result;
  parameter.result = &result;
  TrtFaceBangsUnittest unittest(parameter);
  unittest.test();
}

#endif