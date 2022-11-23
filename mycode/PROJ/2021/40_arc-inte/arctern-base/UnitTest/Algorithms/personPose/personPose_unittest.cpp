/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/personPose/personPose.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH +
         std::string(
             "/../arctern-models/develop/body/pose-predictor-f32-d-0.0.3.bin");
}

TEST(PERSONPOS, Init_NULL) { initNull<arctern::PersonPose>(); }

TEST(PERSONPOS, Init_WRONGParam) {
  initWrongParam<arctern::PersonPose, arctern::InitParameter>(getModelPath());
}

TEST(PERSONPOS, Init_normal) {
  initNormal<arctern::PersonPose>(getModelPath());
}

TEST(PERSONPOS, Release) { release<arctern::PersonPose>(); }

TEST(PERSONPOS, Process_noInit) { processNoInit<arctern::PersonPose>(); }

TEST(PERSONPOS, Process_Init) {
  processInitWithRectV2<arctern::PersonPose, arctern::PersonPoseRunParameter>(
      getModelPath());
}
} // namespace
