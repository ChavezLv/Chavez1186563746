//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "UnitTest/config.h"
#include "gtest/gtest.h"
#include <utility>
#if defined(TRT_ALLOWED)

TEST(ObjectQuality, TRT_CARTEST) {
  TrtObjQualityTestParam parameter;
  parameter.ymlGroundTruth =
      getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/car-quality-d-0.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/car-quality-d-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_car_quality/";
  parameter.modelPath =
      ARCTERN_PROJECT_PATH +
      string("/../arctern-models/develop/vehicle/car-quality-d-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::ObjectQualityResult result;
  parameter.result=&result;
  TrtObjectQualityUnittest unittest(parameter);
  unittest.test();
}

TEST(ObjectQuality, TRT_EBIKETEST) {
  TrtObjQualityTestParam parameter;
  parameter.ymlGroundTruth =
      getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/ebike-quality-d-0.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/ebike-quality-d-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_ebike_quality/";
  parameter.modelPath =
      ARCTERN_PROJECT_PATH +
      string("/../arctern-models/develop/vehicle/ebike-quality-d-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::ObjectQualityResult result;
  parameter.result=&result;
  TrtObjectQualityUnittest unittest(parameter);
  unittest.test();
}

TEST(ObjectQuality, TRT_LICENSETEST) {
  TrtObjQualityTestParam parameter;
  parameter.ymlGroundTruth =
      getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/license-quality-d-0.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/license-quality-d-0.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_license_quality/";
  parameter.modelPath =
      ARCTERN_PROJECT_PATH +
      string("/../arctern-models/develop/vehicle/license-quality-d-0.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::ObjectQualityResult result;
  parameter.result=&result;
  TrtObjectQualityUnittest unittest(parameter);
  unittest.test();
}

#endif
