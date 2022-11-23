//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(FaceDetect, Atlas004) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/multi-detector-tiny-f32-d-0.0.4-rc-atlas.yml";
 parameter.ymlInput =  "";
 parameter.imgPath = getDir() + "data/atlas/multi_det_center/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/multi-detector-center-combined-b1-acl-atlas300-f16-d-0.0.4.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/multi-detector-center-combined-b8-acl-atlas300-f16-d-0.0.4.bin");
 parameter.gpuIndex = 0;
 arctern::MultiDetectCenterResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasMultiDetectCenterUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif