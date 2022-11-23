//
// Created by Admin on 2019/6/16.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "common/release_utils.h"
#include "common/aes_cryptor.h"
#include "common/file_buffer.h"
#if defined(TRT_ALLOWED)
TEST(FaceDetectTiny, TRT_video003) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-d-0.0.3-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_tiny_face/";

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-det-video-d-0.0.3.bin");

#ifndef PLAIN_MODEL
  ///> encrypt model
  GENERATE_RELEASE_KEYS_BLOCK;
  arctern::AESCryptor aes_cryptor(RELEASE_KEY_STRING);
  arctern::FileBuffer file_buffer(parameter.modelPath);
  string planData;
  planData.assign(file_buffer.buffer_,file_buffer.length_);

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-det-video-0.0.3.bin");
  aes_cryptor.encrypt_file(parameter.modelPath,planData);

#endif

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::FaceDetectTinyResult result;
  parameter.result=&result;
  TrtFaceDetectTinyUnittest unittest(parameter);
  unittest.test();
}
TEST(FaceDetectTiny, TRT_onnx) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-f32-1.0.0-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_tiny_face_yolov5/";

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/onnx/face-det-video-f32-d-1.0.0.bin");
#ifndef PLAIN_MODEL
  ///> encrypt model
  GENERATE_RELEASE_KEYS_BLOCK;
  arctern::AESCryptor aes_cryptor(RELEASE_KEY_STRING);
  arctern::FileBuffer file_buffer(parameter.modelPath);
  string planData;
  planData.assign(file_buffer.buffer_,file_buffer.length_);

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/onnx/face-det-video-f32-1.0.0.bin");
  aes_cryptor.encrypt_file(parameter.modelPath,planData);

#endif

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::FaceDetectTinyResult result;
  parameter.result=&result;
  TrtFaceDetectTinyUnittest unittest(parameter);
  unittest.test();
}
/*TEST(FaceDetectTiny, TRT_090) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-0.9.0-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_tiny_face/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-det-0.9.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceDetectTinyResult result;
  parameter.result=&result;
  TrtFaceDetectTinyUnittest unittest(parameter);
  unittest.test();
}
TEST(FaceDetectTiny, TRT_video004) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-f32-d-0.0.4-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_tiny_face/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-det-video-f32-d-0.0.4.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceDetectTinyResult result;
  parameter.result=&result;
  TrtFaceDetectTinyUnittest unittest(parameter);
  unittest.test();
}
TEST(FaceDetectTiny, TRT_video005) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-f32-d-0.0.5-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_tiny_face/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-det-video-f32-d-0.0.5.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceDetectTinyResult result;
  parameter.result=&result;
  TrtFaceDetectTinyUnittest unittest(parameter);
  unittest.test();
}*/
#endif