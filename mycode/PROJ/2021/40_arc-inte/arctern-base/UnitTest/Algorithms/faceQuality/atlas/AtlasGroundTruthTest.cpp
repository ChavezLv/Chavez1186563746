//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(FaceQuality, Atlas103) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-quality-d-1.0.3-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/face-quality-d-1.0.3-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/face_quality/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-quality-b1-acl-atlas300-f16-d-1.0.3.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-quality-b8-acl-atlas300-f16-d-1.0.3.bin");
 parameter.gpuIndex = 0;
 arctern::FaceQualityResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFaceQualityUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif