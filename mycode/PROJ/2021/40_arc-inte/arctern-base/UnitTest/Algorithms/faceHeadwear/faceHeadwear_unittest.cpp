/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceHeadwear/faceHeadwear.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-headwearcolor-helmet-f32-d-0.0.1.bin");
}

TEST(FACEHEADWEAR, Init_NULL) {
  initNull<arctern::FaceHeadwear>();
}

TEST(FACEHEADWEAR, Init_WRONGParam) {
  initWrongParam<arctern::FaceHeadwear, arctern::InitParameter>(getModelPath());
}

TEST(FACEHEADWEAR, Init_normal) {
  initNormal<arctern::FaceHeadwear>(getModelPath());
}

TEST(FACEHEADWEAR, Release) {
  release<arctern::FaceHeadwear>();
}

TEST(FACEHEADWEAR, Process_noInit) {
  processNoInit<arctern::FaceHeadwear>();
}

TEST(FACEHEADWEAR, Process_Init) {
  processInitWithRect<arctern::FaceHeadwear, arctern::FaceHeadwearRunParameter>(getModelPath());
}
}  // namespace
