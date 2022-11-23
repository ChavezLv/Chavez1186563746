/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.16
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/objectQuality/objectQuality.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::ObjectQuality recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 2;
  initParam.model_path = argv[1];
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread(argv[2]);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::ObjectQualityRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  runParameter.rectsV_.push_back({545, 293, 247, 417}); //ebike, 001.png
  runParameter.rectsV_.push_back({545, 293, 247, 417}); //ebike, 001.png

  arctern::ObjectQualityResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "object quality score:"<< result.objectQuality_[0] << std::endl;
  }

  return 0;
}
