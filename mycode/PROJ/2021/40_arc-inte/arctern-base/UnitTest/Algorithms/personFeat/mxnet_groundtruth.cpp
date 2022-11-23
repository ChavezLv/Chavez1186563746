/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(PersonFeat, MXNET_090) {
  std::string inputParamPath = getDir() + "input_params/person-feat-f32-d-0.9.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-f32-d-0.9.0-rc.yml";
  std::string imgDir = getDir() + "data/get_pedestrian_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-feat-f32-d-0.9.0.bin");
  arctern::PersonFeatResult result;
  PersonFeatUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(PersonFeat, MXNET_TINY050) {
  std::string inputParamPath = getDir() + "input_params/person-feat-tiny-f32-d-0.5.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-tiny-f32-d-0.5.0-rc.yml";
  std::string imgDir = getDir() + "data/get_pedestrian_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-feat-tiny-f32-d-0.5.0.bin");
  arctern::PersonFeatResult result;
  PersonFeatUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}


#elif  TVM_ALLOWED
TEST(PersonFeat, TVM_TINY050) {
  std::string inputParamPath = getDir() + "input_params/person-feat-tiny-f32-d-0.5.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-tiny-f32-d-0.5.0-rc.yml";
  std::string imgDir = getDir() + "data/get_pedestrian_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-feat-tiny-tvm-f32-d-0.5.0-var7.bin");
  arctern::PersonFeatResult result;
  PersonFeatUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(PersonFeat, TVM_040) {
  std::string inputParamPath = getDir() + "input_params/person-feat-0.4.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-0.4.0-rc.yml";
  std::string imgDir = getDir() + "data/get_pedestrian_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-feat-tvm-f32-d-0.4.0-var7.bin");
  arctern::PersonFeatResult result;
  PersonFeatUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(PersonFeat, TVM_090) {
  std::string inputParamPath = getDir() + "input_params/person-feat-f32-d-0.9.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-f32-d-0.9.0-rc.yml";
  std::string imgDir = getDir() + "data/get_pedestrian_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-feat-tvm-f32-d-0.9.0-var7.bin");
  arctern::PersonFeatResult result;
  PersonFeatUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(PersonFeat, TVM_100) {
  std::string inputParamPath = getDir() + "input_params/person-feat-f32-d-1.0.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/person-feat-f32-d-1.0.0-rc.yml";
  std::string imgDir = getDir() + "data/get_pedestrian_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-feat-tvm-f32-d-1.0.0-var7.bin");
  arctern::PersonFeatResult result;
  PersonFeatUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}



#endif