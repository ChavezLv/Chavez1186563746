/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/faceIdentify/faceIdentify.h"

namespace {

std::string getModelPath() {

#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH +
         std::string("/../arctern-models/develop/face/face-identity-d-0.0.1.bin");
#endif
}

TEST(FaceIdentify, Init_NULL) { initNull<arctern::FaceIdentify>(); }

TEST(FaceIdentify, Init_WRONGParam) {
  initWrongParam<arctern::FaceIdentify, arctern::InitParameter>(getModelPath());
}

TEST(FaceIdentify, Init_normal) { initNormal<arctern::FaceIdentify>(getModelPath()); }

TEST(FaceIdentify, Release) { release<arctern::FaceIdentify>(); }

TEST(FaceIdentify, Parameter_NoInit) { paramNoInit<arctern::FaceIdentify>(); }

TEST(FaceIdentify, Process_noInit) { processNoInit<arctern::FaceIdentify>(); }

TEST(FaceIdentify, Process_Init) {
  processInitWithRect<arctern::FaceIdentify, arctern::FaceIdentifyRunParameter>(
      getModelPath());
}
} // namespace
