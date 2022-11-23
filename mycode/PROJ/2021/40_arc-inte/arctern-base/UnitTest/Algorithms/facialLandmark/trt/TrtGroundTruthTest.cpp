//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)

TEST(FaceLandmark, TRT130) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-landmark-d-1.3.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-landmark-d-1.3.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_facial_landmark/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-landmark-d-1.3.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FacialLandmarkResult result;
  parameter.result=&result;
  TrtFacialLandmarkUnittest unittest(parameter);
  unittest.test();
}

/*TEST(FaceLandmark, TRT140) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-landmark-f32-d-1.4.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-landmark-f32-d-1.4.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_facial_landmark/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-landmark-f32-d-1.4.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FacialLandmarkResult result;
  parameter.result=&result;
  TrtFacialLandmarkUnittest unittest(parameter);
  unittest.test();
}*/

#endif