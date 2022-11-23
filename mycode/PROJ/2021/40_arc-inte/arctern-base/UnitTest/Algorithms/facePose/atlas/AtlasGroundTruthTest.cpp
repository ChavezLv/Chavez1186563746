//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(FacePose, Atlas100) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-pose-cls-d-1.0.0-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/face-pose-cls-d-1.0.0-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/face_pose/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-pose-cls-b1-acl-atlas300-f16-d-1.0.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-pose-cls-b8-acl-atlas300-f16-d-1.0.0.bin");
 parameter.gpuIndex = 0;
 arctern::FacePoseResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFacePoseUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif