/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/ageGender/ageGender.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {

#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH
      + std::string("/../arctern-models/develop/face/face-gender-age-d-0.1.0.bin");
#elif defined(TVM_ALLOWED)
  return ARCTERN_PROJECT_PATH
      + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gender-age-tvm-f32-d-0.2.0-var7.bin");
#endif
}

TEST(AGEGender, Init_NULL) {
  initNull<arctern::AgeGender>();
}

TEST(AGEGender, Init_WRONGParam) {
  initWrongParam<arctern::AgeGender, arctern::InitParameter>(getModelPath());
}

TEST(AGEGender, Init_normal) {
  initNormal<arctern::AgeGender>(getModelPath());
}

TEST(AGEGender, Release) {
  release<arctern::AgeGender>();
}

TEST(AGEGender, Process_noInit) {
  processNoInit<arctern::AgeGender>();
}

TEST(AGEGender, Process_Init) {
  processInitWithRect<arctern::AgeGender,
              arctern::AgeGenderRunParameter>(getModelPath());
}
}  // namespace
