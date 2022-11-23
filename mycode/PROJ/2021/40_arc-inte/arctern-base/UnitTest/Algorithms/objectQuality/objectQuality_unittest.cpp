/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.16
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/objectQuality/objectQuality.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::ObjectQuality;
using arctern::ObjectQuality;
using arctern::ObjectQualityRunParameter;

namespace {
std::string getModelPath() {
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/ebike-quality-d-0.0.0.bin");
  return modelPath;
}

TEST(ObjectQuality, Init_NULL) {
  initNull<arctern::ObjectQuality>();
}

TEST(ObjectQuality, Init_WRONGParam) {
  initWrongParam<arctern::ObjectQuality, arctern::InitParameter>(getModelPath());
}

TEST(ObjectQuality, Init_normal) {
  initNormal<arctern::ObjectQuality>(getModelPath());
}

TEST(ObjectQuality, Release) {
  release<arctern::ObjectQuality>();
}

TEST(ObjectQuality, Process_noInit) {
  processNoInit<arctern::ObjectQuality>();
}

TEST(ObjectQuality, Process_Init) {
  processInitWithRect<arctern::ObjectQuality, arctern::ObjectQualityRunParameter>(getModelPath());
}
}  // namespace
