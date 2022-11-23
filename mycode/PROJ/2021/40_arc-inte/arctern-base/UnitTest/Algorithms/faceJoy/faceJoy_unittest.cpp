/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/faceJoy/faceJoy.h"

namespace {

std::string getModelPath() {

#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH +
         std::string("/../arctern-models/develop/face/face-joy-1.6.0.bin");
#endif
}

TEST(FACEJOY, Init_NULL) { initNull<arctern::FaceJoy>(); }

TEST(FACEJOY, Init_WRONGParam) {
  initWrongParam<arctern::FaceJoy, arctern::InitParameter>(getModelPath());
}

TEST(FACEJOY, Init_normal) { initNormal<arctern::FaceJoy>(getModelPath()); }

TEST(FACEJOY, Release) { release<arctern::FaceJoy>(); }

TEST(FACEJOY, Process_noInit) { processNoInit<arctern::FaceJoy>(); }

TEST(FACEJOY, Process_Init) {
  processInitWithRect<arctern::FaceJoy, arctern::FaceJoyRunParameter>(
      getModelPath());
}
} // namespace
