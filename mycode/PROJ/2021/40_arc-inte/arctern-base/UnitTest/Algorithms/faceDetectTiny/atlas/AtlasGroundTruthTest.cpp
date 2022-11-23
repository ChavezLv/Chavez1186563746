//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(FaceDetect, Atlas003) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/face-det-video-d-0.0.3-rc-atlas.yml";
 parameter.ymlInput =  "";
 parameter.imgPath = getDir() + "data/atlas/face_detTiny/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/atlas/face-det-video-b1-acl-atlas300-f16-d-0.0.3.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/atlas/face-det-video-b8-acl-atlas300-f16-d-0.0.3.bin");
 parameter.gpuIndex = 0;
 arctern::FaceDetectTinyResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasFaceDetectTinyUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif