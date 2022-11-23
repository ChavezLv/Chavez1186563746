//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)

TEST(VehicleFeat, Atlas090) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/vehicle-feat-d-0.0.1-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/vehicle-feat-d-0.0.1-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/vehicle_feat/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/vehicle-feat-b1-acl-atlas300-f16-d-0.0.1-var1.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/vehicle-feat-b8-acl-atlas300-f16-d-0.0.1-var1.bin");
 parameter.gpuIndex = 0;
 arctern::vehicleFeatResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasVehicleFeatUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}
#endif