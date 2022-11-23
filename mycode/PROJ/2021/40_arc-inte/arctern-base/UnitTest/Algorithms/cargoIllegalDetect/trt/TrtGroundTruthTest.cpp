//
// Created by Admin on 2021/12/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "common/release_utils.h"
#include "common/aes_cryptor.h"
#include "common/file_buffer.h"

#if defined(TRT_ALLOWED)
TEST(CargoIllegalDetect, TRT_video003) {

  TrtTestParam parameter;
  /* parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/cargo-illegal-det-f32-d-0.1.0.json"; */
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/cargoIllegal-det-f32-d-0.1.0-rc.json";
std::cout << ymlPath << std:: endl;
  parameter.ymlInput = "";
  /* parameter.imgPath = getDir() + "data/detect_cargo_illegal/"; */
  std::string imgDir = getDir() + "data/cargo_illegal_det_010/";
std::cout << imgDir << std:: endl;

  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/unclassified/cargoIllegal-det-f32-d-0.1.0.bin");
std::cout << modelPath << std:: endl;
  /* parameter.modelPath = ARCTERN_PROJECT_PATH + \ */
  /*            string("/../arctern-models/develop/unclassified/cargo-illegal-det-f32-d-0.1.0.bin"); */

#ifndef PLAIN_MODEL
  ///> encrypt model
  GENERATE_RELEASE_KEYS_BLOCK;
  arctern::AESCryptor aes_cryptor(RELEASE_KEY_STRING);
  arctern::FileBuffer file_buffer(parameter.modelPath);
  string planData;
  planData.assign(file_buffer.buffer_,file_buffer.length_);

  parameter.modelPath = ARCTERN_PROJECT_PATH + \
      std::string("/../arctern-models/develop/unclassified/cargoIllegal-det-f32-d-0.1.0.bin");
  aes_cryptor.encrypt_file(parameter.modelPath,planData);

#endif

  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::CargoIllegalDetectResult result;
  parameter.result=&result;
  TrtCargoIllegalDetectUnittest unittest(parameter);
  unittest.test();
}
#endif
