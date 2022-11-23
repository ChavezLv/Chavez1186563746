/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.01
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "src/Algorithms/policeSuit/policeSuit.h"
#include <istream>

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::PoliceSuit recog;
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

  arctern::PoliceSuitRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  // runParameter.imageV_.push_back(img);

  /// face rect
  runParameter.rectsV_.push_back({597, 105, 223, 223});
  // runParameter.rectsV_.push_back({598, 109, 233, 233});

  arctern::PoliceSuitResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.m_vSuitResults[0].first<< std::endl;
  }

  return 0;
}
