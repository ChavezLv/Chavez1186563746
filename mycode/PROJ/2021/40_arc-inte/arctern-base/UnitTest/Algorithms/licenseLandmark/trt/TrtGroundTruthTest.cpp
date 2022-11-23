//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(LicenseLandmark, TRT100) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/license-landmark-f32-d-1.0.0-rc.json";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/licenseRecog/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/vehicle/license-landmark-f32-d-1.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::LicenseLandmarkResult result;
  parameter.result=&result;
  TrtLicenseLandmarkUnittest unittest(parameter);
  unittest.test();
}


#endif