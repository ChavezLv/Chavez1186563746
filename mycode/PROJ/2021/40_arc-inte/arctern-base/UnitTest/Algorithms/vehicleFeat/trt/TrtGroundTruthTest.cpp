//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(VehicleFeat, TRT001) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/vehicle-feat-d-0.0.1-rc.yml";
  parameter.ymlInput = getDir() + "input_params/vehicle-feat-d-0.0.1-i.yml";
  parameter.imgPath = getDir() + "data/get_vehicle_feat/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/vehicle/vehicle-feat-d-0.0.1.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::vehicleFeatResult result;
  parameter.result=&result;
  TrtVehicleFeatUnittest unittest(parameter);
  unittest.test();
}


#endif