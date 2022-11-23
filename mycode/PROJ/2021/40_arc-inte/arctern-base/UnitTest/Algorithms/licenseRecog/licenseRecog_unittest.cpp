/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/licenseRecog/licenseRecog.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/license-recog-lite-f32-d-0.1.0.bin");
}

TEST(LicenseRecog, Init_NULL) {
  initNull<arctern::LicenseRecog>();
}

TEST(LicenseRecog, Init_WRONGParam) {
  initWrongParam<arctern::LicenseRecog, arctern::InitParameter>(getModelPath());
}

TEST(LicenseRecog, Init_normal) {
  initNormal<arctern::LicenseRecog>(getModelPath());
}

TEST(LicenseRecog, Release) {
  release<arctern::LicenseRecog>();
}

TEST(LicenseRecog, Process_noInit) {
  processNoInit<arctern::LicenseRecog>();
}

TEST(LicenseRecog, Process_Init) {
  processInitWithRect<arctern::LicenseRecog, arctern::LicenseRecogRunParameter>(getModelPath());
}
}  // namespace
