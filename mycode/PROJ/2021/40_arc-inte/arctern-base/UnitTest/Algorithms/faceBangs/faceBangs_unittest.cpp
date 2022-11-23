/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceBangs/faceBangs.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-glass-1.0.3.bin");
}

TEST(FACEBANGS, Init_NULL) {
  initNull<arctern::FaceBangs>();
}

TEST(FACEBANGS, Init_WRONGParam) {
  initWrongParam<arctern::FaceBangs, arctern::InitParameter>(getModelPath());
}

TEST(FACEBANGS, Init_normal) {
  initNormal<arctern::FaceBangs>(getModelPath());
}

TEST(FACEBANGS, Release) {
  release<arctern::FaceBangs>();
}

TEST(FACEBANGS, Process_noInit) {
  processNoInit<arctern::FaceBangs>();
}

TEST(FACEBANGS, Process_Init) {
  processInitWithRect<arctern::FaceBangs, arctern::FaceBangsRunParameter>(getModelPath());
}
}  // namespace
