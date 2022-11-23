/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personAttr/personAttr.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-attr-d-0.2.0.bin");
}

TEST(PERSONATTR, Init_NULL) {
  initNull<arctern::PersonAttr>();
}

TEST(PERSONATTR, Init_WRONGParam) {
  initWrongParam<arctern::PersonAttr, arctern::InitParameter>(getModelPath());
}

TEST(PERSONATTR, Init_normal) {
  initNormal<arctern::PersonAttr>(getModelPath());
}

TEST(PERSONATTR, Release) {
  release<arctern::PersonAttr>();
}

TEST(PERSONATTR, Process_noInit) {
  processNoInit<arctern::PersonAttr>();
}

TEST(PERSONATTR, Process_Init) {
  processInitWithRect<arctern::PersonAttr, arctern::PersonAttrRunParameter>(getModelPath());
}
}  // namespace
