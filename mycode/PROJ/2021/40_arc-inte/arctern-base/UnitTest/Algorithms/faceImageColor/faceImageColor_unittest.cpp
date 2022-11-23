/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceImageColor/faceImageColor.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH;

#ifdef MXNET_ALLOWED
 // modelPath += std::string("/../arctern-models/develop/face/face-mask-d-0.2.1.bin");
#elif defined(HISI_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/nnie3516/face-image-color-nnie-1.0.1.bin");
#elif defined(TVM_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_image_color_1.0.2/face-image-color-f32-d-1.0.2.bin");
#elif defined(RKNN_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/rknn/face-image-color-1.0.1.rknn");
#endif

  return modelPath;
}

TEST(FaceImageColor, Init_NULL) {
  initNull<arctern::FaceImageColor>();
}

TEST(FaceImageColor, Init_WRONGParam) {
  initWrongParam<arctern::FaceImageColor, arctern::InitParameter>(getModelPath());
}

TEST(FaceImageColor, Init_normal) {
  initNormal<arctern::FaceImageColor>(getModelPath());
}

TEST(FaceImageColor, Release) {
  release<arctern::FaceImageColor>();
}

//TEST(FaceImageColor, Parameter_NoInit) {
//  paramNoInit<arctern::FaceImageColor>();
//}

TEST(FaceImageColor, Process_noInit) {
  processNoInit<arctern::FaceImageColor>();
}

TEST(FaceImageColor, Process_Init) {
  processInitWithRect<arctern::FaceImageColor, arctern::FaceImageColorRunParameter>(getModelPath());
}
}  // namespace
