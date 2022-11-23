//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(PersonCompleteness, Atlas000) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/person-completeness-0.0.0-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/person-completeness-0.0.0-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/person_completeness/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-completeness-b1-acl-atlas300-f16-d-0.0.0-var1.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-completeness-b8-acl-atlas300-f16-d-0.0.0-var1.bin");
 parameter.gpuIndex = 0;
 arctern::PersonCompletenessResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasPersonCompletenessUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif