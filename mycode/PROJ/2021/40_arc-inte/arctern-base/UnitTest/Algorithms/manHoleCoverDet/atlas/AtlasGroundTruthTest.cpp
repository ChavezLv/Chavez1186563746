//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)

TEST(ManHoleCover, Atlas020) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/manholecover-det-d-0.2.0-rc-atlas.json";
 parameter.ymlInput =  "";
 parameter.imgPath = getDir() + "data/atlas/manhole_cover_abnormal_det_010/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/manholeCover-abnormal-det-b1-acl-atlas300-f16-d-0.2.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/manholeCover-abnormal-det-b8-acl-atlas300-f16-d-0.2.0.bin");
 parameter.gpuIndex = 0;
 arctern::ManHoleCoverDetResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasManHoleCoverDetUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}

// TEST(ManHoleCover, Atlas010) {
 
//  AtlasTestParam parameter;
//  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/manholecover-det-d-0.1.0-rc-atlas.yml";
//  parameter.ymlInput =  "";
//  parameter.imgPath = getDir() + "data/atlas/manholecover_abnormal/";
//  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/manholeCover-abnormal-det-b1-acl-atlas300-f16-d-0.1.0.bin");
//  parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/manholeCover-abnormal-det-b8-acl-atlas300-f16-d-0.1.0.bin");
//  parameter.gpuIndex = 0;
//  arctern::ManHoleCoverDetResult result;
//  parameter.result=&result;
 
//  auto ret = initAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret, true);
//  AtlasManHoleCoverDetUnittest unittest(parameter);
//  unittest.test();
//  ret = destroyAclDeviceEnv(parameter.gpuIndex);
//  ASSERT_EQ(ret,true);
// }


#endif