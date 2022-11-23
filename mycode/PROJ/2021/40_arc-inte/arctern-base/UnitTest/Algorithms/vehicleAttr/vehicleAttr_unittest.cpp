/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/vehicleAttr/vehicleAttr.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/vehicle-attributes-f32-d-0.3.0-var1.bin");
}

TEST(VEHICLEATTR, Init_NULL) {
  initNull<arctern::VehicleAttr>();
}

TEST(VEHICLEATTR, Init_WRONGParam) {
  initWrongParam<arctern::VehicleAttr, arctern::InitParameter>(getModelPath());
}

TEST(VEHICLEATTR, Init_normal) {
  initNormal<arctern::VehicleAttr>(getModelPath());
}

TEST(VEHICLEATTR, Release) {
  release<arctern::VehicleAttr>();
}

TEST(VEHICLEATTR, Process_noInit) {
  processNoInit<arctern::VehicleAttr>();
}

TEST(VEHICLEATTR, Process_Init) {
  processInitWithRect<arctern::VehicleAttr, arctern::VehicleAttrRunParameter>(getModelPath());
}
}  // namespace
