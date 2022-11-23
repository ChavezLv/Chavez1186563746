/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/faceType/faceType.h"

namespace {

std::string getModelPath() {

#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH +
         std::string("/../arctern-models/develop/face/face-type2-0.2.0.bin");
#endif
}

TEST(faceType, Init_NULL) { initNull<arctern::FaceType>(); }

TEST(faceType, Init_WRONGParam) {
  initWrongParam<arctern::FaceType, arctern::InitParameter>(getModelPath());
}

TEST(faceType, Init_normal) { initNormal<arctern::FaceType>(getModelPath()); }

TEST(faceType, Release) { release<arctern::FaceType>(); }


TEST(faceType, Process_noInit) { processNoInit<arctern::FaceType>(); }

TEST(faceType, Process_Init) {
  processInitWithRect<arctern::FaceType, arctern::FaceTypeRunParameter>(
      getModelPath());
}
} // namespace
