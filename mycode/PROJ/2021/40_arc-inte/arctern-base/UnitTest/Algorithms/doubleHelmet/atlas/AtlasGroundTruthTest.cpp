//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(DoubleHelmet, Atlas010) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/double-helmet-d-0.1.0-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/double-helmet-d-0.1.0-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/double_helmet/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/double-helmet-b1-acl-atlas300-f16-d-0.1.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/double-helmet-b8-acl-atlas300-f16-d-0.1.0.bin");
 parameter.gpuIndex = 0;
 arctern::DoubleHelmetResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasDoubleHelmetUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif