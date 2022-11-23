//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


// TEST(PersonFeatTiny, Atlas050) {
 
//  AtlasTestParam parameter;
//  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/person-feat-tiny-f32-d-0.5.0-rc-atlas.yml";
//  parameter.ymlInput = getDir() + "input_params/atlas/person-feat-tiny-f32-d-0.5.0-i-atlas.yml";
//  parameter.imgPath = getDir() + "data/atlas/person_feat/";
//  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-feat-tiny-b1-acl-atlas300-f16-d-0.5.0-var1.bin");
//  parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-feat-tiny-b8-acl-atlas300-f16-d-0.5.0-var1.bin");
//  parameter.gpuIndex = 0;
//  arctern::PersonFeatResult result;
//  parameter.result=&result;
 
//  auto ret = initAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret, true);
//  AtlasPersonFeatUnittest unittest(parameter);
//  unittest.test();
//  ret = destroyAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret,true);
// }

TEST(PersonFeat, Atlas090) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/person-feat-f32-d-0.9.0-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/person-feat-f32-d-0.9.0-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/person_feat/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-feat-b1-acl-atlas300-f16-d-0.9.0-var1.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-feat-b8-acl-atlas300-f16-d-0.9.0-var1.bin");
 parameter.gpuIndex = 0;
 arctern::PersonFeatResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasPersonFeatUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}

#endif