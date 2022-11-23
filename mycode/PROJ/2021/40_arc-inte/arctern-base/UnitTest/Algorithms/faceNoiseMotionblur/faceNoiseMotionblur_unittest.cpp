/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceNoiseMotionblur/faceNoiseMotionblur.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH;

#ifdef MXNET_ALLOWED
  modelPath += std::string("/../arctern-models/develop/face/face-noise-motionblur-classifier-f32-d-0.0.9.bin");
#elif defined(HISI_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/nnie3516/face-noise-motionblur-classifier-nnie3516-i8-q1-d-0.0.8.bin");
#elif defined(RKNN_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/rknn/face-noise-motionblur-classifier-0.0.8.rknn");
#elif defined(TVM_ALLOWED) && defined(__arm__)
  modelPath += std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_noise_motionblur_0.0.8/face-noise-motionblur-rk3288C-f32-d-0.0.8.bin");

#elif defined(TVM_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-noise-motionblur-classifier-tvm-f32-d-0.0.9-var7.bin");
#endif

  return modelPath;
}

TEST(FaceNoiseMotionblur, Init_NULL) {
  initNull<arctern::FaceNoiseMotionblur>();
}

TEST(FaceNoiseMotionblur, Init_WRONGParam) {
  initWrongParam<arctern::FaceNoiseMotionblur, arctern::InitParameter>(getModelPath());
}

TEST(FaceNoiseMotionblur, Init_normal) {
  initNormal<arctern::FaceNoiseMotionblur>(getModelPath());
}

TEST(FaceNoiseMotionblur, Release) {
  release<arctern::FaceNoiseMotionblur>();
}

TEST(FaceNoiseMotionblur, Process_noInit) {
  processNoInit<arctern::FaceNoiseMotionblur>();
}

TEST(FaceNoiseMotionblur, Process_Init) {
  // ALG alg;
  processInitWithRect<arctern::FaceNoiseMotionblur,
              arctern::FaceNoiseMotionblurRunParameter>(getModelPath());
}
}  // namespace
