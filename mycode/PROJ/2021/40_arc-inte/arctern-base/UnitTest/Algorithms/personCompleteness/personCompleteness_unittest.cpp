/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.07
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personCompleteness/personCompleteness.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-completeness-0.0.0.bin");
#elif defined(TVM_ALLOWED)
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-completeness-tvm-f32-d-0.0.0-var7.bin");
#endif
}

TEST(PersonCompleteness, Init_NULL) {
  initNull<arctern::PersonCompleteness>();
}

TEST(PersonCompleteness, Init_WRONGParam) {
  initWrongParam<arctern::PersonCompleteness, arctern::InitParameter>(getModelPath());
}

TEST(PersonCompleteness, Init_normal) {
  initNormal<arctern::PersonCompleteness>(getModelPath());
}

TEST(PersonCompleteness, Release) {
  release<arctern::PersonCompleteness>();
}

TEST(PersonCompleteness, Process_noInit) {
  processNoInit<arctern::PersonCompleteness>();
}

TEST(PersonCompleteness, Process_Init) {
  processInitWithRect<arctern::PersonCompleteness, arctern::PersonCompletenessRunParameter>(getModelPath());
}
}  // namespace
