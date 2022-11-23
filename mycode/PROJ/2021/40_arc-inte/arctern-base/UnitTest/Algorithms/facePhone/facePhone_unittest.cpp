/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/facePhone/facePhone.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-phone-f32-d-0.0.1.bin");
}

TEST(FacePhone, Init_NULL) {
  initNull<arctern::FacePhone>();
}

TEST(FacePhone, Init_WRONGParam) {
  initWrongParam<arctern::FacePhone, arctern::InitParameter>(getModelPath());
}

TEST(FacePhone, Init_normal) {
  initNormal<arctern::FacePhone>(getModelPath());
}

TEST(FacePhone, Release) {
  release<arctern::FacePhone>();
}

TEST(FacePhone, Process_noInit) {
  processNoInit<arctern::FacePhone>();
}

TEST(FacePhone, Process_Init) {
  processInitWithRect<arctern::FacePhone, arctern::FacePhoneRunParameter>(getModelPath());
}
}  // namespace
