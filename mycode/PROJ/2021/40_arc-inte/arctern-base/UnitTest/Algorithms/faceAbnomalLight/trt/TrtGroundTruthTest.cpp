//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


/*TEST(FaceAbnomalLight, TRT002) {
 TrtTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnomal-light-classifier-d-0.0.2-rc.yml";
 parameter.ymlInput = getDir() + "input_params/face-abnomal-light-classifier-d-0.0.2-i.yml";
 parameter.imgPath = getDir() + "data/predict_face_abnomal_light/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-abnomal-light-classifier-d-0.0.2.bin");
 parameter.irModelPath = "trt_mid_model_file.test";
 parameter.gpuIndex = 0;
parameter.result=&result;
 TrtFaceAbnomalLightUnittest unittest(parameter);
 unittest.test();
}

TEST(FaceAbnomalLight, TRT003) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnormal-light-classifier-f32-d-0.0.3-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-abnormal-light-classifier-f32-d-0.0.3-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_abnomal_light/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-abnormal-light-classifier-f32-d-0.0.3.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
parameter.result=&result;
  TrtFaceAbnomalLightUnittest unittest(parameter);
  unittest.test();
}*/

TEST(FaceAbnomalLight, TRT004) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnormal-light-classifier-f32-d-0.0.4-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-abnormal-light-classifier-f32-d-0.0.4-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_abnomal_light/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-abnormal-light-classifier-f32-d-0.0.4.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceAbnomalLightResult face_abnomal_light_result;
  parameter.result = &face_abnomal_light_result;
  TrtFaceAbnomalLightUnittest unittest(parameter);
  unittest.test();
}


#endif