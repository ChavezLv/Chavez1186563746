/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.03
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personQuality/personQuality.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::PersonQuality recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = argv[1];
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread(argv[2]);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::PersonQualityRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  //runParameter.rectsV_.push_back({178, 18, 502, 595});
  runParameter.rectsV_.push_back({87,0,204,728});

  arctern::PersonQualityResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "person quality score:"<< result.personQuality_[0] << std::endl;
  }

  return 0;
}
