/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceDetectTiny/faceDetectTiny.h"
#include "../../base-unittest.h"
#include "../../config.h"
namespace {

std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH;

#ifdef MXNET_ALLOWED
  modelPath += std::string("/../arctern-models/develop/face/face-det-tiny-f32-d-2.0.0.bin");
#elif defined(HISI_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/nnie3516/face-det-tiny-nnie3516-i8-q1-d-2.0.0.bin");
#elif defined(TVM_ALLOWED)
  modelPath += std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-det-video-tvm-f32-d.0.0.3.bin");
#endif

  return modelPath;
}



//TEST(FaceDetectTiny, Init_NULL) {
//  initNull<arctern::FaceDetectTiny>();
//}
//
//TEST(FaceDetectTiny, Init_WRONGParam) {
//  initWrongParam<arctern::FaceDetectTiny, arctern::InitParameter>(getModelPath());
//}
//
//TEST(FaceDetectTiny, Init_normal) {
//  initNormal<arctern::FaceDetectTiny>(getModelPath());
//}
//
//TEST(FaceDetectTiny, Release) {
//  release<arctern::FaceDetectTiny>();
//}
//
//TEST(FaceDetectTiny, Parameter_Init) {
//  arctern::FaceDetectTiny det;
//
//  /// init
//  ::arctern::InitParameter initParam;
//  initParam.model_path = getModelPath();
//  initParam.batchNum_ = 2;
//  initParam.thresh_ = 0.5;
//  EXPECT_EQ(det.Init(&initParam), arctern::ErrorType::ERR_SUCCESS);
//}
//
//TEST(FaceDetectTiny, Process_noInit) {
//  processNoInit<arctern::FaceDetectTiny>();
//}
//
//TEST(FaceDetectTiny, Process_Init) {
//  processInitCommon<arctern::FaceDetectTiny, arctern::RunParameter>(getModelPath());
//}
}  // namespace



