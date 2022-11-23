/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.13
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_BASE_UNITTEST_H_
#define UNITTEST_BASE_UNITTEST_H_

#include <string>

#include "include/initparameter.h"
#include "include/runparameter.h"

#include "gtest/gtest.h"

template<typename ALG>
void initNull() {
  ALG alg;
  EXPECT_EQ(alg.Init(nullptr), arctern::ErrorType::ERR_INIT_PARAM);
}

template<typename ALG, typename PARAM>
void initWrongParam(std::string modelPath) {

  ALG alg;
  PARAM initParam;

  EXPECT_EQ(alg.Init(&initParam), arctern::ErrorType::ERR_INIT_PARAM);

  initParam.batchNum_ = -1;
  initParam.model_path = modelPath;
  EXPECT_EQ(alg.Init(&initParam), arctern::ErrorType::ERR_INIT_PARAM);

  initParam.model_path = "****";
  initParam.batchNum_ = 2;
  arctern::ErrorType wrongPathRet = alg.Init(&initParam);
  EXPECT_EQ(wrongPathRet, arctern::ErrorType::ERR_INVALID_MODEL);
}

template<typename ALG>
void initNormal(std::string modelPath) {

  ALG alg;
  ::arctern::InitParameter initParam;
  initParam.model_path = modelPath;
  initParam.batchNum_ = 1;
  arctern::ErrorType ret = alg.Init(&initParam);
  std::cout << "Init OK"<<std::endl;

  std::cout << "return = "<<ret <<  std::endl;
  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

}

template<typename ALG>
void release() {
  ALG alg;
  alg.Release();
}


template<typename ALG>
void processNoInit() {
  ALG alg;
  auto ret = alg.Process(nullptr,nullptr);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_UNEXPECT);
}

using namespace arctern;

template<typename ALG>
void processInit(ALG & alg) {

  auto processRet = alg.Process(nullptr, nullptr);
  EXPECT_EQ(processRet, ErrorType::ERR_NO_IMAGES);

  arctern::RunParameter runParam;
  processRet = alg.Process(&runParam, nullptr);
  EXPECT_EQ(processRet, ErrorType::ERR_NO_IMAGES);
}

template<typename ALG, typename ALGRunner>
void processInitCommon(std::string modelPath) {
  ALG alg;

  /// init
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = modelPath;

  EXPECT_EQ(alg.Init(&initParam), ErrorType::ERR_SUCCESS);

  processInit<ALG>(alg);

  ALGRunner runParam2;
  arctern::ArcternImage image;
  runParam2.imageV_.push_back(image);
  auto processRet = alg.Process(&runParam2, nullptr);
  EXPECT_EQ(processRet, ErrorType::ERR_NO_IMAGE_DATA);
}

template<typename ALG, typename ALGRunner>
void processInitWithRect(std::string modelPath) {
  ALG alg;
  using namespace arctern;

  /// init
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = modelPath;

  ErrorType ret = alg.Init(&initParam);
  EXPECT_EQ(ret, ErrorType::ERR_SUCCESS);
  if(ret != ErrorType::ERR_SUCCESS) return;
  processInit<ALG>(alg);

  //////there is one image, but no face rect!!
  ALGRunner runParam2;
  arctern::ArcternImage image;
  runParam2.imageV_.push_back(image);

  auto processRet = alg.Process(&runParam2, nullptr);
  EXPECT_EQ(processRet, ErrorType::ERR_RUN_PARM);

  ALGRunner runParam3;
  arctern::ArcternRect rect;
  runParam3.imageV_.push_back(image);
  runParam3.rectsV_.push_back(rect);
  processRet = alg.Process(&runParam3, nullptr);
  EXPECT_EQ(processRet, ErrorType::ERR_NO_IMAGE_DATA);
}

template<typename ALG, typename ALGRunner>
void processInitWithRectV2(std::string modelPath) {
  ALG alg;
  using namespace arctern;

  /// init
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = modelPath;
  ErrorType ret = alg.Init(&initParam);
  EXPECT_EQ(ret, ErrorType::ERR_SUCCESS);
  if(ret != ErrorType::ERR_SUCCESS) return;
  processInit<ALG>(alg);

  //////there is one image, but no face rect!!
  ALGRunner runParam2;
  arctern::ArcternImage image;
  runParam2.imageV_.push_back(image);

  auto processRet = alg.Process(&runParam2, nullptr);

  EXPECT_EQ(processRet, ErrorType::ERR_NO_IMAGE_DATA);

  ALGRunner runParam3;
  arctern::ArcternRect rect;
  runParam3.imageV_.push_back(image);
  runParam3.rectsV_.push_back(rect);
  processRet = alg.Process(&runParam3, nullptr);
  EXPECT_EQ(processRet, ErrorType::ERR_NO_IMAGE_DATA);
}
#endif  // UNITTEST_BASE_UNITTEST_H_
