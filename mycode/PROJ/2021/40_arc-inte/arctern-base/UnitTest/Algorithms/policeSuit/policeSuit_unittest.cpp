/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/policeSuit/policeSuit.h"

namespace {

std::string getModelPath() {

#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH +
         std::string("/../arctern-models/develop/face/"
                     "police-suit-predictor-f32-d-0.0.0.bin");
#endif
}

TEST(PoliceSuit, Init_NULL) { initNull<arctern::PoliceSuit>(); }

TEST(PoliceSuit, Init_WRONGParam) {
  initWrongParam<arctern::PoliceSuit, arctern::InitParameter>(getModelPath());
}

TEST(PoliceSuit, Init_normal) {
  initNormal<arctern::PoliceSuit>(getModelPath());
}

TEST(PoliceSuit, Release) { release<arctern::PoliceSuit>(); }

TEST(PoliceSuit, Parameter_NoInit) { paramNoInit<arctern::PoliceSuit>(); }

TEST(PoliceSuit, Process_noInit) { processNoInit<arctern::PoliceSuit>(); }

TEST(PoliceSuit, Process_Init) {
  processInitWithRect<arctern::PoliceSuit, arctern::PoliceSuitRunParameter>(
      getModelPath());
}
} // namespace
