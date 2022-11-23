/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.18
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceOcclusion/faceOcclusion.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::FaceOcclusion;
using arctern::FaceOcclusionResult;
using arctern::FaceOcclusionRunParameter;

namespace {
std::string getModelPath() {

#ifdef MXNET_ALLOWED
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-occlusion-detection-d-0.0.1.bin");
#elif defined(TVM_ALLOWED)
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-occlusion-detection-tvm-f32-d-0.0.1-var7.bin");
#endif
  return modelPath;
}

TEST(FaceOcclusion, Init_NULL) {
  initNull<arctern::FaceOcclusion>();
}

TEST(FaceOcclusion, Init_WRONGParam) {
  initWrongParam<arctern::FaceOcclusion, arctern::InitParameter>(getModelPath());
}

TEST(FaceOcclusion, Init_normal) {
  initNormal<arctern::FaceOcclusion>(getModelPath());
}

TEST(FaceOcclusion, Release) {
  release<arctern::FaceOcclusion>();
}

TEST(FaceOcclusion, Process_noInit) {
  processNoInit<arctern::FaceOcclusion>();
}

TEST(FaceOcclusion, Process_Init) {
  processInitWithRect<arctern::FaceOcclusion, arctern::FaceOcclusionRunParameter>(getModelPath());
}
}  // namespace
