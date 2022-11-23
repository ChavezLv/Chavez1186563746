//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(VehicleAttr, Atlas031) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/vehicle-attributes-f32-d-0.3.1-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/vehicle-attributes-f32-d-0.3.1-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/vehicle_attr/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/vehicle-attributes-b1-acl-atlas300-f16-d-0.3.1.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/vehicle-attributes-b8-acl-atlas300-f16-d-0.3.1.bin");
 parameter.gpuIndex = 0;
 arctern::VehicleAttrResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasVehicleAttrUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif