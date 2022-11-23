//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceHat, TRT103) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-hat-1.0.3-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-hat-1.0.3-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_hat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-hat-1.0.3.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceHatResult result;
  parameter.result=&result;
  TrtFaceHatUnittest unittest(parameter);
  unittest.test();
}

#endif