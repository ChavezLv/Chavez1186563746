//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(FacialLandmark, Atlas130) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-landmark-d-1.3.0-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/face-landmark-d-1.3.0-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/face_landmark/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-landmark-b1-acl-atlas300-f16-d-1.3.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-landmark-b8-acl-atlas300-f16-d-1.3.0.bin");
 parameter.gpuIndex = 0;
 arctern::FacialLandmarkResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFacialLandmarkUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


// TEST(FacialLandmark, Atlas140) {
 
//  AtlasTestParam parameter;
//  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-landmark-d-1.3.0-rc-atlas.yml";
//  parameter.ymlInput = getDir() + "input_params/atlas/face-landmark-d-1.3.0-i-atlas.yml";
//  parameter.imgPath = getDir() + "data/atlas/face_landmark/";
//  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-landmark-b1-acl-atlas300-f16-d-1.4.0.bin");
//  parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-landmark-b8-acl-atlas300-f16-d-1.4.0.bin");
//  parameter.gpuIndex = 0;
//  arctern::FacialLandmarkResult result;
//  parameter.result=&result;
 
//  auto ret = initAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret, true);
//  AtlasFacialLandmarkUnittest unittest(parameter);
//  unittest.test();
//  ret = destroyAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret,true);
// }
#endif