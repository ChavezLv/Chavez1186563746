//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


//TEST(FaceGluon, Atlas300) {
 
//  AtlasTestParam parameter;
//  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-gluon-d-3.0.0-rc-atlas.yml";
//  parameter.ymlInput = getDir() + "input_params/atlas/face-gluon-d-3.0.0-i-atlas.yml";
//  parameter.imgPath = getDir() + "data/atlas/face_gluon/";
//  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-gluon-b1-acl-atlas300-f16-d-3.0.0.bin");
//  parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-gluon-b8-acl-atlas300-f16-d-3.0.0.bin");
//  parameter.gpuIndex = 0;
//  arctern::FaceGluonResult result;
//  parameter.result=&result;
 
//  auto ret = initAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret, true);
//  AtlasFaceGluonUnittest unittest(parameter);
//  unittest.test();
//  ret = destroyAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret,true);
// }

TEST(FaceGluon, Atlas450) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-gluon-d-4.5.0-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/face-gluon-d-4.5.0-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/face_gluon/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-gluon-b1-acl-atlas300-f16-d-4.5.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-gluon-b8-acl-atlas300-f16-d-4.5.0.bin");
 parameter.gpuIndex = 0;
 arctern::FaceGluonResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFaceGluonUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}

#endif