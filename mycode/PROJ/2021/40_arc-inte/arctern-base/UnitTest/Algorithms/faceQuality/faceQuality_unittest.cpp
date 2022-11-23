/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceQuality/faceQuality.h"
#include "../../base-unittest.h"
#include "../../config.h"

using arctern::FaceQuality;
using arctern::FaceQuality;
using arctern::FaceQualityRunParameter;

namespace {
std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH;

#ifdef MXNET_ALLOWED
  modelPath += std::string("/../arctern-models/develop/face/face-quality-d-1.0.3.bin");
#elif defined(HISI_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/nnie3516/face-quality-nnie3516-i8-q1-d-1.0.3.bin");
#elif defined(RKNN_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/rknn/face-quality-1.0.3.rknn");
#elif defined(TVM_ALLOWED) && defined(__arm__)
  modelPath += std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_quality_1.0.2/face-quality-f32-d-1.0.2.bin");
#elif defined(TVM_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-quality-tvm-f32-d-1.0.3-var7.bin");
#endif

  return modelPath;
}

TEST(FaceQuality, Init_NULL) {
  initNull<arctern::FaceQuality>();
}

TEST(FaceQuality, Init_WRONGParam) {
  initWrongParam<arctern::FaceQuality, arctern::InitParameter>(getModelPath());
}

TEST(FaceQuality, Init_normal) {
  initNormal<arctern::FaceQuality>(getModelPath());
}

TEST(FaceQuality, Release) {
  release<arctern::FaceQuality>();
}

TEST(FaceQuality, Process_noInit) {
  processNoInit<arctern::FaceQuality>();
}

TEST(FaceQuality, Process_Init) {
  processInitWithRect<arctern::FaceQuality, arctern::FaceQualityRunParameter>(getModelPath());
}
}  // namespace
