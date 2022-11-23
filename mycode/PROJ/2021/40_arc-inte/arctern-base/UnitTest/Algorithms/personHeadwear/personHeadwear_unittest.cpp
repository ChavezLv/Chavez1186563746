/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.02
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/



#include "src/Algorithms/personHeadwear/personHeadwear.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-headwear-f32-d-0.0.0.bin");
}

TEST(PersonHeadwear, Init_NULL) {
  initNull<arctern::PersonHeadwear>();
}

TEST(PersonHeadwear, Init_WRONGParam) {
  initWrongParam<arctern::PersonHeadwear, arctern::InitParameter>(getModelPath());
}

TEST(PersonHeadwear, Init_normal) {
  initNormal<arctern::PersonHeadwear>(getModelPath());
}

TEST(PersonHeadwear, Release) {
  release<arctern::PersonHeadwear>();
}

TEST(PersonHeadwear, Process_noInit) {
  processNoInit<arctern::PersonHeadwear>();
}

TEST(PersonHeadwear, Process_Init) {
  processInitWithRectV2<arctern::PersonHeadwear, arctern::PersonHeadwearRunParameter>(getModelPath());
}
}  // namespace
