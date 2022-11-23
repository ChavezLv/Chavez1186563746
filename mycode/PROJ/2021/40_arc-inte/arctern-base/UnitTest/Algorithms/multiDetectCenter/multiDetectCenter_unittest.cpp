/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/multiDetectCenter/multiDetectCenter.h"
#include "../../base-unittest.h"
#include "../../config.h"
namespace {

std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/unclassified/multi-detector-center-combined-f32-d-0.0.2.bin");
  return modelPath;
}

TEST(MultiDetectCenter, Init_NULL) {
  initNull<arctern::MultiDetectCenter>();
}

TEST(MultiDetectCenter, Init_WRONGParam) {
  initWrongParam<arctern::MultiDetectCenter, arctern::InitParameter>(getModelPath());
}

TEST(MultiDetectCenter, Init_normal) {
  initNormal<arctern::MultiDetectCenter>(getModelPath());
}

TEST(MultiDetectCenter, Release) {
  release<arctern::MultiDetectCenter>();
}


TEST(MultiDetectCenter, Parameter_Init) {
  arctern::MultiDetectCenter det;

  /// init
  ::arctern::InitParameter initParam;
  initParam.model_path = getModelPath();
  initParam.batchNum_ = 2;
  initParam.thresh_ = 0.5;
  EXPECT_EQ(det.Init(&initParam), arctern::ErrorType::ERR_SUCCESS);
}

TEST(MultiDetectCenter, Process_noInit) {
  processNoInit<arctern::MultiDetectCenter>();
}

TEST(MultiDetectCenter, Process_Init) {
  processInitCommon<arctern::MultiDetectCenter, arctern::RunParameter>(getModelPath());
}
}  // namespace



