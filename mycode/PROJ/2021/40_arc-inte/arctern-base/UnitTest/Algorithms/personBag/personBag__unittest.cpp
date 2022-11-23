/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/personBag/personBag.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH +
         std::string(
             "/../arctern-models/develop/body/person-bag-f32-d-0.0.0.bin");
}

TEST(PERSONBAG, Init_NULL) { initNull<arctern::PersonBag>(); }

TEST(PERSONBAG, Init_WRONGParam) {
  initWrongParam<arctern::PersonBag, arctern::InitParameter>(getModelPath());
}

TEST(PERSONBAG, Init_normal) { initNormal<arctern::PersonBag>(getModelPath()); }

TEST(PERSONBAG, Release) { release<arctern::PersonBag>(); }

TEST(PERSONBAG, Parameter_NoInit) { paramNoInit<arctern::PersonBag>(); }

TEST(PERSONBAG, Process_noInit) { processNoInit<arctern::PersonBag>(); }

TEST(PERSONBAG, Process_Init) {
  processInitWithRect<arctern::PersonBag, arctern::PersonBagRunParameter>(
      getModelPath());
}
} // namespace
