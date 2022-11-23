//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "UnitTest/config.h"
#include "gtest/gtest.h"
#include <utility>

#if defined(TRT_ALLOWED)

TEST(FaceIdentify, TRT001) {
  TrtFaceIdentifyParam parameter;
  parameter.ymlGroundTruth =
      getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-identity-d-0.0.1-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-identity-d-0.0.1-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_identity/";
  parameter.modelPath =
      ARCTERN_PROJECT_PATH +
      string("/../arctern-models/develop/face/face-identity-d-0.0.1.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceIdentifyResult result;
  parameter.result=&result;
  TrtFaceIdentifyUnittest unittest(parameter);
  unittest.test();
}

#endif
