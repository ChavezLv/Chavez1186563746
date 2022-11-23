/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceLivenessRgb/faceLivenessRgb.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::FaceLivenessRgb;
using arctern::FaceLivenessRgbRunParameter;

namespace {
std::string getModelPath() {
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-liveness-ir-d-1.0.4.bin");
  return modelPath;
}

TEST(FaceLivenessIr, Init_NULL) {
  initNull<arctern::FaceLivenessRgb>();
}

TEST(FaceLivenessIr, Init_WRONGParam) {
  initWrongParam<arctern::FaceLivenessRgb, arctern::InitParameter>(getModelPath());
}

TEST(FaceLivenessIr, Init_normal) {
  initNormal<arctern::FaceLivenessRgb>(getModelPath());
}

TEST(FaceLivenessIr, Release) {
  release<arctern::FaceLivenessRgb>();
}

TEST(FaceLivenessIr, Process_noInit) {
  processNoInit<arctern::FaceLivenessRgb>();
}

TEST(FaceLivenessIr, Process_Init) {
  processInitWithRect<arctern::FaceLivenessRgb, arctern::FaceLivenessRgbRunParameter>(getModelPath());
}
}  // namespace
