/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/vehicleCycleFeat/vehicleCycleFeat.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH +
         std::string("/../arctern-models/develop/vehicle/" 
                     "vehicle-cycle-feat-f32-d-0.0.1.bin");
}

TEST(VEHICLEATTR, Init_NULL) { initNull<arctern::VehicleCycleFeat>(); }

TEST(VEHICLEATTR, Init_WRONGParam) {
  initWrongParam<arctern::VehicleCycleFeat, arctern::InitParameter>(getModelPath());
}

TEST(VEHICLEATTR, Init_normal) {
  initNormal<arctern::VehicleCycleFeat>(getModelPath());
}

TEST(VEHICLEATTR, Release) { release<arctern::VehicleCycleFeat>(); }

TEST(VEHICLEATTR, Parameter_NoInit) { paramNoInit<arctern::VehicleCycleFeat>(); }

TEST(VEHICLEATTR, Process_noInit) { processNoInit<arctern::VehicleCycleFeat>(); }

TEST(VEHICLEATTR, Process_Init) {
  processInitWithRect<arctern::VehicleCycleFeat, arctern::VehicleCycleFeatRunParameter>(
      getModelPath());
}
} // namespace
