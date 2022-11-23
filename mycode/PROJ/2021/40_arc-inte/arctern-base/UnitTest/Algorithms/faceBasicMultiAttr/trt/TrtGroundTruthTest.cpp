//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceBasicMultiAttr, TRT001) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-multi-attributes-f32-d-0.0.1-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-multi-attributes-f32-d-0.0.1-i.yml";
  parameter.imgPath = getDir() + "data/predict_facebasic_multiattributes/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-multi-attributes-f32-d-0.0.1.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceBasicMultiAttrResult result;
  parameter.result=&result;
  TrtFaceBasicMultiAttrUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceBasicMultiAttr, TRT011) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-multi-attributes-f32-d-0.1.1-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-multi-attributes-f32-d-0.1.1-i.yml";
  parameter.imgPath = getDir() + "data/predict_facebasic_multiattributes/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-multi-attributes-f32-d-0.1.1.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceBasicMultiAttrResult result;
  parameter.result=&result;
  TrtFaceBasicMultiAttrUnittest unittest(parameter);
  unittest.test();
}

#endif