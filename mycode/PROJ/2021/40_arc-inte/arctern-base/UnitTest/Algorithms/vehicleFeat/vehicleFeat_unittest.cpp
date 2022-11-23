/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/vehicleFeat/vehicleFeat.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH +
         std::string(
             "/../arctern-models/develop/vehicle/vehicle-feat-d-0.0.1.bin");
}

TEST(VEHICLEATTR, Init_NULL) { initNull<arctern::vehicleFeat>(); }

TEST(VEHICLEATTR, Init_WRONGParam) {
  initWrongParam<arctern::vehicleFeat, arctern::InitParameter>(getModelPath());
}

TEST(VEHICLEATTR, Init_normal) {
  initNormal<arctern::vehicleFeat>(getModelPath());
}

TEST(VEHICLEATTR, Release) { release<arctern::vehicleFeat>(); }

TEST(VEHICLEATTR, Parameter_NoInit) { paramNoInit<arctern::vehicleFeat>(); }

TEST(VEHICLEATTR, Process_noInit) { processNoInit<arctern::vehicleFeat>(); }

TEST(VEHICLEATTR, Process_Init) {
  processInitWithRect<arctern::vehicleFeat, arctern::vehicleFeatRunParameter>(
      getModelPath());
}
} // namespace
