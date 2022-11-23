/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.25
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceBasicMultiAttr/faceBasicMultiAttr.h"
#include "../../base-unittest.h"
#include "../../config.h"

namespace {

std::string getModelPath() {
#ifdef MXNET_ALLOWED
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-multi-attributes-f32-d-0.0.1.bin");
#elif TVM_ALLOWED
  return ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-multi-attributes-tvm-f32-d-0.1.1-var7.bin");
#endif
}

TEST(FACEBASICMULTIATTR, Init_NULL) {
  initNull<arctern::FaceBasicMultiAttr>();
}

TEST(FACEBASICMULTIATTR, Init_WRONGParam) {
  initWrongParam<arctern::FaceBasicMultiAttr, arctern::InitParameter>(getModelPath());
}

TEST(FACEBASICMULTIATTR, Init_normal) {
  initNormal<arctern::FaceBasicMultiAttr>(getModelPath());
}

TEST(FACEBASICMULTIATTR, Release) {
  release<arctern::FaceBasicMultiAttr>();
}

TEST(FACEBASICMULTIATTR, Process_noInit) {
  processNoInit<arctern::FaceBasicMultiAttr>();
}

TEST(FACEBASICMULTIATTR, Process_Init) {
  processInitWithRect<arctern::FaceBasicMultiAttr, arctern::FaceBasicMultiAttrRunParameter>(getModelPath());
}
}  // namespace
