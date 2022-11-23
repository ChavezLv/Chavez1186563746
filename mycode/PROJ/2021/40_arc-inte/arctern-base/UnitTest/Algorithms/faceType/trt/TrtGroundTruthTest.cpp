//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceType, TRT010) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-type-0.1.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-type-0.1.0-i.yml";
  parameter.imgPath = getDir() + "/data/predict_face_type/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-type-0.1.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceTypeResult result;
  parameter.result=&result;
  TrtFaceTypeUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceType, TRT020) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-type2-0.2.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-type2-0.2.0-i.yml";
  parameter.imgPath = getDir() + "/data/predict_face_type2/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-type2-0.2.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceTypeResult result;
  parameter.result=&result;
  TrtFaceTypeUnittest unittest(parameter);
  unittest.test();
}

#endif