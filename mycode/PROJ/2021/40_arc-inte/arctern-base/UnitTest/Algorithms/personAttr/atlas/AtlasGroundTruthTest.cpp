//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


// TEST(PersonAttr, Atlas050) {
 
//  AtlasTestParam parameter;
//  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/person-attr-d-0.5.0-rc-atlas.yaml";
//  parameter.ymlInput = getDir() + "input_params/atlas/person-attr-d-0.5.0-i-atlas.yaml";
//  parameter.imgPath = getDir() + "data/atlas/person_attribute/";
//  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-attr-b1-acl-atlas300-f16-d-0.5.0.bin");
//  parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-attr-b8-acl-atlas300-f16-d-0.5.0.bin");
//  parameter.gpuIndex = 0;
//  arctern::PersonAttrResult result;
//  parameter.result=&result;
 
//  auto ret = initAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret, true);
//  AtlasPersonAttrUnittest unittest(parameter);
//  unittest.test();
//  ret = destroyAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret,true);
// }

TEST(PersonAttr, Atlas060) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/person-attr-d-0.6.0-rc-atlas.yaml";
 parameter.ymlInput = getDir() + "input_params/atlas/person-attr-d-0.6.0-i-atlas.yaml";
 parameter.imgPath = getDir() + "data/atlas/person_attribute/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-attr-b1-acl-atlas300-f16-d-0.6.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/person-attr-b8-acl-atlas300-f16-d-0.6.0.bin");
 parameter.gpuIndex = 0;
 arctern::PersonAttrResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasPersonAttrUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif