/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceSmoke/faceSmoke.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/smoke-d-0.0.1.bin");
}

TEST(FaceSmoke, Init_NULL) {
  initNull<arctern::FaceSmoke>();
}

TEST(FaceSmoke, Init_WRONGParam) {
  initWrongParam<arctern::FaceSmoke, arctern::InitParameter>(getModelPath());
}

TEST(FaceSmoke, Init_normal) {
  initNormal<arctern::FaceSmoke>(getModelPath());
}

TEST(FaceSmoke, Release) {
  release<arctern::FaceSmoke>();
}

TEST(FaceSmoke, Process_noInit) {
  processNoInit<arctern::FaceSmoke>();
}

TEST(FaceSmoke, Process_Init) {
  processInitWithRect<arctern::FaceSmoke, arctern::FaceSmokeRunParameter>(getModelPath());
}
}  // namespace
