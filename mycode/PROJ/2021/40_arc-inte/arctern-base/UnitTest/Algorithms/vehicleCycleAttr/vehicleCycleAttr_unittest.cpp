/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/vehicleCycleAttr/vehicleCycleAttr.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/vehicle/vehicle-cycle-attributes-f32-d-0.0.3.bin");
}

TEST(VEHICLECYCLEATTR, Init_NULL) {
  initNull<arctern::VehicleCycleAttr>();
}

TEST(VEHICLECYCLEATTR, Init_WRONGParam) {
  initWrongParam<arctern::VehicleCycleAttr, arctern::InitParameter>(getModelPath());
}

TEST(VEHICLECYCLEATTR, Init_normal) {
  initNormal<arctern::VehicleCycleAttr>(getModelPath());
}

TEST(VEHICLECYCLEATTR, Release) {
  release<arctern::VehicleCycleAttr>();
}

TEST(VEHICLECYCLEATTR, Process_noInit) {
  processNoInit<arctern::VehicleCycleAttr>();
}

TEST(VEHICLECYCLEATTR, Process_Init) {
  processInitWithRect<arctern::VehicleCycleAttr, arctern::VehicleCycleAttrRunParameter>(getModelPath());
}

}  // namespace
