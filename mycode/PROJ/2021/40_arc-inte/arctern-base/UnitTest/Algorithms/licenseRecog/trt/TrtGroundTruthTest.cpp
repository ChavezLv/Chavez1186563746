//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(LicenseRecog, TRT010) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/license-recog-lite-f32-d-0.1.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/license-recog-lite-f32-d-0.1.0-i.yml";
  parameter.imgPath = getDir() + "data/license_recognition_lite/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/vehicle/license-recog-lite-f32-d-0.1.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::LicenseRecogResult result;
  parameter.result=&result;
  TrtLicenseRecogUnittest unittest(parameter);
  unittest.test();
}

TEST(LicenseRecog, TRT200) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/license-recog-f32-d-2.0.0-rc.json";
  parameter.ymlInput = getDir() + "input_params/license-recog-f32-d-2.0.0-json";
  parameter.imgPath = getDir() + "data/licenseRecog/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/vehicle/license-recog-f32-d-2.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::LicenseRecogResult result;
  parameter.result=&result;
  TrtLicenseRecogUnittest unittest(parameter);
  unittest.test();
}
#endif