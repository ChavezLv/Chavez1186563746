//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


// TEST(FaceBasicMultiAttr, Atlas011) {
 
//  AtlasTestParam parameter;
//  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-multi-attributes-f32-d-0.1.1-rc-atlas.yml";
//  parameter.ymlInput = getDir() + "input_params/atlas/face-multi-attributes-f32-d-0.1.1-i-atlas.yml";
//  parameter.imgPath = getDir() + "data/atlas/face_multi_attributes/";
//  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-multi-attributes-b1-acl-atlas300-f16-d-0.1.1.bin");
//  parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-multi-attributes-b8-acl-atlas300-f16-d-0.1.1.bin");
//  parameter.gpuIndex = 0;
//  arctern::FaceBasicMultiAttrResult result;
//  parameter.result=&result;
 
//  auto ret = initAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret, true);
//  AtlasFaceBasicMultiAttrUnittest unittest(parameter);
//  unittest.test();
//  ret = destroyAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret,true);
// }


TEST(FaceBasicMultiAttr, Atlas011) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-multi-attributes-f32-q2-d-0.1.0-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/face-multi-attributes-f32-q2-d-0.1.0-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/face_multi_attributes/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-multi-attributes-b1-acl-atlas300-f16-q2-d-0.1.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-multi-attributes-b8-acl-atlas300-f16-q2-d-0.1.0.bin");
 parameter.gpuIndex = 0;
 arctern::FaceBasicMultiAttrResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFaceBasicMultiAttrUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif