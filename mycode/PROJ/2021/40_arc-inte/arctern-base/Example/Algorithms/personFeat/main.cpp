/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personFeat/personFeat.h"
#include "src/Algorithms/personPose/personPose.h"
#include "src/Algorithms/multiDetectCenter/multiDetectCenter.h"
#include "opencv2/opencv.hpp"
#include "cassert"
void Test(char **argv){
  arctern::PersonFeat recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.deviceType_ = arctern::ARCTERN_TRT;
  initParam.model_path = argv[1];
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread(argv[2]);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  img.type = arctern::CPU;


  arctern::PersonFeatRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  /// preson rect
  runParameter.rectsV_.push_back({0, 0, 110, 221});

  arctern::PersonFeatResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.features_[0].size() << std::endl;
  }
}
int main(int argc, char **argv) {

  return 0;
}
