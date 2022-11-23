/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/



#include "src/Algorithms/faceMask/faceMask.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH;

#ifdef MXNET_ALLOWED
  modelPath += std::string("/../arctern-models/develop/face/face-mask-d-0.2.1.bin");
#elif defined(HISI_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/nnie3516/face-mask-nnie3516-i8-d-0.2.1.bin");
#elif defined(RKNN_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/rknn/face-mask-0.2.1.rknn");
#elif defined(TVM_ALLOWED) && defined(__arm__)
  modelPath += std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_face_mask_0.2.1/face-mask-f32-d-0.2.1.bin");

#elif defined(TVM_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-mask-tvm-f32-d-0.2.1-var7.bin");
#endif

  return modelPath;
}

TEST(FaceMask, Init_NULL) {
  initNull<arctern::FaceMask>();
}

TEST(FaceMask, Init_WRONGParam) {
  initWrongParam<arctern::FaceMask, arctern::InitParameter>(getModelPath());
}

TEST(FaceMask, Init_normal) {
  initNormal<arctern::FaceMask>(getModelPath());
}

TEST(FaceMask, Release) {
  release<arctern::FaceMask>();
}

TEST(FaceMask, Process_noInit) {
  processNoInit<arctern::FaceMask>();
}

TEST(FaceMask, Process_Init) {
  processInitWithRect<arctern::FaceMask, arctern::FaceMaskRunParameter>(getModelPath());
}
}  // namespace
