//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(FaceSmoke, Atlas001) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/smoke-d-0.0.1-rc-atlas.yml";
 parameter.ymlInput = getDir() + "input_params/atlas/smoke-d-0.0.1-i-atlas.yml";
 parameter.imgPath = getDir() + "data/atlas/face_smoke/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-smoke-b1-acl-atlas300-f16-d-0.0.1.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/face-smoke-b8-acl-atlas300-f16-d-0.0.1-var2.bin");
 parameter.gpuIndex = 0;
 arctern::FaceSmokeResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFaceSmokeUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif