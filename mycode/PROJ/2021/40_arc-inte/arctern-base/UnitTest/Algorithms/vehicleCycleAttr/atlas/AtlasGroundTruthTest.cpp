//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(VehicleCycleAttr, Atlas003) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/vehicle-cycle-attributes-f32-d-0.0.3-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/vehicle-cycle-attributes-f32-d-0.0.3-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/vehicle_cycle_attr/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/vehicle-cycle-attributes-b1-acl-atlas300-f16-d-0.0.3.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/vehicle-cycle-attributes-b8-acl-atlas300-f16-d-0.0.3.bin");
 parameter.gpuIndex = 0;
 arctern::VehicleCycleAttrResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasVehicleCycleAttrUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif