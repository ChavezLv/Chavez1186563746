//
// Created by Admin on 2021/12/8.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "common/release_utils.h"
#include "common/aes_cryptor.h"
#include "common/file_buffer.h"


#if defined(TRT_ALLOWED)

TEST(RubbishDetectTiny, TRT_video000) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/expose_rubbish-det-f32-d-0.1.0-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_rubbish_tiny/";

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/expose_rubbish-det-f32-d-0.1.0.bin");

#ifndef PLAIN_MODEL
  ///> encrypt model
  GENERATE_RELEASE_KEYS_BLOCK;
  arctern::AESCryptor aes_cryptor(RELEASE_KEY_STRING);
  arctern::FileBuffer file_buffer(parameter.modelPath);
  string planData;
  planData.assign(file_buffer.buffer_,file_buffer.length_);

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/expose_rubbish-det-f32-d-0.1.0.bin");
  aes_cryptor.encrypt_file(parameter.modelPath,planData);

#endif

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::RubbishDetectTinyResult result;
  parameter.result=&result;
  TrtRubbishDetectTinyUnittest unittest(parameter);
  unittest.test();
}
TEST(RubbishDetectTiny, TRT_onnx) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/expose_rubbish-det-f32-d-0.1.0-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_banner/";

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/expose_rubbish-det-f32-d-0.1.0.bin");
#ifndef PLAIN_MODEL
  ///> encrypt model
  GENERATE_RELEASE_KEYS_BLOCK;
  arctern::AESCryptor aes_cryptor(RELEASE_KEY_STRING);
  arctern::FileBuffer file_buffer(parameter.modelPath);
  string planData;
  planData.assign(file_buffer.buffer_,file_buffer.length_);

  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/expose_rubbish-det-f32-d-0.1.0.bin");
  aes_cryptor.encrypt_file(parameter.modelPath,planData);

#endif

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::RubbishDetectTinyResult result;
  parameter.result=&result;
  TrtRubbishDetectTinyUnittest unittest(parameter);
  unittest.test();
}

#endif
