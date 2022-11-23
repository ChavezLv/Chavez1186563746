/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.07
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "../../base-unittest.h"
#include "../../config.h"
#include "src/Algorithms/dogDetTiny/dogDetTiny.h"

namespace {

std::string getModelPath() {
  std::string modelPath = ARCTERN_PROJECT_PATH;

  modelPath += std::string(
      "/../arctern-models/develop/unclassified/dog-det-video-f32-d-0.0.0.bin");

  return modelPath;
}

TEST(dogDetTiny, Init_NULL) { initNull<arctern::dogDetTiny>(); }

TEST(dogDetTiny, Init_WRONGParam) {
  initWrongParam<arctern::dogDetTiny, arctern::InitParameter>(getModelPath());
}

TEST(dogDetTiny, Init_normal) {
  initNormal<arctern::dogDetTiny>(getModelPath());
}

TEST(dogDetTiny, Release) { release<arctern::dogDetTiny>(); }


TEST(dogDetTiny, Parameter_Init) {
  arctern::dogDetTiny det;

  /// init
  ::arctern::InitParameter initParam;
  initParam.model_path = getModelPath();
  initParam.batchNum_ = 2;
  initParam.thresh_ = 0.5;
  EXPECT_EQ(det.Init(&initParam), arctern::ErrorType::ERR_SUCCESS);

}

TEST(dogDetTiny, Process_noInit) { processNoInit<arctern::dogDetTiny>(); }

TEST(dogDetTiny, Process_Init) {
  processInitCommon<arctern::dogDetTiny, arctern::RunParameter>(getModelPath());
}
} // namespace
