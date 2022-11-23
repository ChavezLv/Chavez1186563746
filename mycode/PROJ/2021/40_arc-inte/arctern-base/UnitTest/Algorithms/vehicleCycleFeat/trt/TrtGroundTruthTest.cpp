//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include <utility>

#if defined(TRT_ALLOWED)

TEST(TrtVehicleCycleFeat, TRT001) {
 TrtVehicleAttrParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/vehicle-cycle-feat-f32-d-0.0.1-rc.yml";
 parameter.ymlInput = getDir() + "input_params/vehicle-cycle-feat-f32-d-0.0.1-i.yml";
 parameter.imgPath = getDir() + "data/get_vehicle_cycle_feat/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/vehicle/vehicle-cycle-feat-f32-d-0.0.1.bin");
 parameter.irModelPath = "trt_mid_model_file.test";
 parameter.gpuIndex = 0;
 arctern::VehicleCycleFeatResult result;
 parameter.result=&result;
 TrtVehicleCycleFeatUnittest unittest(parameter);
 unittest.test();
}

#endif
