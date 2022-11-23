//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(VehicleAttr, TRT031) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/vehicle-attributes-f32-d-0.3.1-rc.yml";
  parameter.ymlInput = getDir() + "input_params/vehicle-attributes-f32-d-0.3.1-i.yml";
  parameter.imgPath = getDir() + "data/predict_car_attributes/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/vehicle/vehicle-attributes-f32-d-0.3.1.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::VehicleAttrResult result;
  parameter.result=&result;
  TrtVehicleAttrUnittest unittest(parameter);
  unittest.test();
}



#endif