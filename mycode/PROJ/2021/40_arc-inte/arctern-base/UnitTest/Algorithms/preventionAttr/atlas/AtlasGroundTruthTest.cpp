//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if ATLAS_ALLOWED



TEST(PreventionAttr, Atlas011) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/prevention-attributes-f32-d-0.1.0-rc.json";
 parameter.ymlInput = getDir() + "input_params/atlas/prevention-attributes-f32-d-0.1.0-i.json";
 parameter.imgPath = getDir() + "data/atlas/prevention_attr/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/prevention-attributes-b1-acl-atlas300-f16-d-0.1.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/prevention-attributes-b8-acl-atlas300-f16-d-0.1.0.bin");
 parameter.gpuIndex = 0;
 arctern::PreventionAttrResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasPreventionAttrUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif