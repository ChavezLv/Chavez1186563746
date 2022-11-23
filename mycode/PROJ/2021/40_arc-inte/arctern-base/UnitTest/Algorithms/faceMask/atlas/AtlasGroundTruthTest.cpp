//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(FaceMask, Atlas021) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-mask-d-0.2.1-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/face-mask-d-0.2.1-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/face_mask/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-mask-b1-acl-atlas300-f16-d-0.2.1.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-mask-b8-acl-atlas300-f16-d-0.2.1.bin");
 parameter.gpuIndex = 0;
 arctern::FaceMaskResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFaceMaskUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif