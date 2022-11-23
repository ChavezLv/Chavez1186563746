/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.03
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personQuality/personQuality.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::PersonQuality;
using arctern::PersonQuality;
using arctern::PersonQualityRunParameter;

namespace {
std::string getModelPath() {
#ifdef MXNET_ALLOWED
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/person-quality-d-1.0.3.bin");
#elif defined(TVM_ALLOWED)
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/body/tvm0.7/avx2/person-quality-tvm-f32-d-1.0.3-var7.bin");
#endif
  return modelPath;
}

TEST(PersonQuality, Init_NULL) {
  initNull<arctern::PersonQuality>();
}

TEST(PersonQuality, Init_WRONGParam) {
  initWrongParam<arctern::PersonQuality, arctern::InitParameter>(getModelPath());
}

TEST(PersonQuality, Init_normal) {
  initNormal<arctern::PersonQuality>(getModelPath());
}

TEST(PersonQuality, Release) {
  release<arctern::PersonQuality>();
}

TEST(PersonQuality, Process_noInit) {
  processNoInit<arctern::PersonQuality>();
}

TEST(PersonQuality, Process_Init) {
  processInitWithRectV2<arctern::PersonQuality, arctern::PersonQualityRunParameter>(getModelPath());
}
}  // namespace
