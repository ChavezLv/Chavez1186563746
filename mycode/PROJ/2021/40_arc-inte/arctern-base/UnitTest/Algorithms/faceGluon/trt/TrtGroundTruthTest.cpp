//
// Created by Admin on 2019/6/15.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)
TEST(FaceGluon, TRT_741) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/face-gluon-f32-d-7.4.1-i.json";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.4.1-rc.json";
  parameter.imgPath = getDir() + "data/face_gluon_7.3/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.4.1.bin");

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_740) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/face-gluon-f32-d-7.4.0-i.json";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.4.0-rc.json";
  parameter.imgPath = getDir() + "data/face_gluon_7.3/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.4.0.bin");

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_730) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/face-gluon-f32-d-7.3.0-i.json";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.3.0-rc.json";
  parameter.imgPath = getDir() + "data/face_gluon_7.3/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.3.0.bin");

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_300) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-gluon-d-3.0.0-i.yml";
  parameter.imgPath = getDir() + "data/get_face_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-gluon-d-3.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_380) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.8.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-gluon-f32-d-3.8.0-i.yml";
  parameter.imgPath = getDir() + "data/get_face_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-gluon-f32-d-3.8.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_390) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.9.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-gluon-f32-d-3.9.0-i.yml";
  parameter.imgPath = getDir() + "data/get_face_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-gluon-f32-d-3.9.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}


TEST(FaceGluon, TRT_720) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/face-gluon-f32-d-7.2.0-i.yml";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.2.0-rc.yml";
  parameter.imgPath = getDir() + "data/get_face_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.2.0.bin");

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_710) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/face-gluon-f32-d-7.1.0-i.yml";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.1.0-rc.yml";
  parameter.imgPath = getDir() + "data/get_face_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.1.0.bin");

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_335) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/face-gluon-d-3.3.5-i.yml";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.3.5-rc.yml";
  parameter.imgPath = getDir() + "data/get_face_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-3.3.5.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}

TEST(FaceGluon, TRT_450) {
  TrtTestParam parameter;
  parameter.ymlInput = getDir() + "input_params/face-gluon-d-4.5.0-i.yml";
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-4.5.0-rc.yml";
  parameter.imgPath = getDir() + "data/get_face_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-4.5.0.bin");

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGluonResult result;
  parameter.result=&result;
  TrtFaceGluonUnittest unittest(parameter);
  unittest.test();
}


#endif