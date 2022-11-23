//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(VehicleCycleAttr, TRT020) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/vehicle-cycle-attributes-f32-d-0.1.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/vehicle-cycle-attributes-f32-d-0.1.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_vehicle_cycle_attributes/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/vehicle/vehicle-cycle-attributes-f32-d-0.1.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::VehicleCycleAttrResult result;
  parameter.result=&result;
  TrtVehicleCycleAttrUnittest unittest(parameter);
  unittest.test();
}



#endif