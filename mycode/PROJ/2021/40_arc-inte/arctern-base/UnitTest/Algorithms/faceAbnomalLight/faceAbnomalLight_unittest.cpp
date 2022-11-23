/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <UnitTest/config.h>
#include "src/Algorithms/faceAbnomalLight/faceAbnomalLight.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {

#ifdef MXNET_ALLOWED
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-abnomal-light-classifier-d-0.0.2.bin");
#elif defined(TVM_ALLOWED)
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-abnormal-light-classifier-tvm-f32-d-0.0.4-var7.bin");
#elif defined(TRT_ALLOWED)
    std::string modelPath =
            ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-abnomal-light-classifier-d-0.0.2.bin");
#endif
  return modelPath;
}

TEST(FaceAbnomalLight, Init_NULL) {
  initNull<arctern::FaceAbnomalLight>();
}

TEST(FaceAbnomalLight, Init_WRONGParam) {
  initWrongParam<arctern::FaceAbnomalLight, arctern::InitParameter>(getModelPath());
}

TEST(FaceAbnomalLight, Init_normal) {
  initNormal<arctern::FaceAbnomalLight>(getModelPath());
}

TEST(FaceAbnomalLight, Release) {
  release<arctern::FaceAbnomalLight>();
}

TEST(FaceAbnomalLight, Process_noInit) {
  processNoInit<arctern::FaceAbnomalLight>();
}

TEST(FaceAbnomalLight, Process_Init) {
  processInitWithRect<arctern::FaceAbnomalLight,
              arctern::FaceAbnomalLightRunParameter>(getModelPath());
}
}  // namespace
