//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"
#include "include/atlasInit.h"
#if defined(ATLAS_ALLOWED)


TEST(MessyMaterials, Atlas000) {
 
 AtlasTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ARMLINUX_ATLAS/messy-materials-d-0.0.0-rc-atlas.yml";
 parameter.ymlInput =  "";
 parameter.imgPath = getDir() + "data/atlas/messy_materials/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/messy-materials-det-b1-acl-atlas300-f16-d-0.0.0.bin");
 parameter.modelPath8 = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/atlas-models/messy-materials-det-b8-acl-atlas300-f16-d-0.0.0.bin");
 parameter.gpuIndex = 0;
 arctern::MessyMaterialsResult result;
 parameter.result=&result;
 
 auto ret = initAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret, true);
 AtlasMessyMaterialsUnittest unittest(parameter);
 unittest.test();
 ret = destroyAclDeviceEnv(parameter.gpuIndex);
 ASSERT_EQ(ret,true);
}


#endif