/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshne
 *  Last modified:  2021.06.04
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceType/faceType.h"
#include <istream>

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::FaceType recog;
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

  arctern::FaceTypeRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  // runParameter.imageV_.push_back(img);

  /// face rect
  runParameter.rectsV_.push_back({597, 105, 223, 223});
  // runParameter.rectsV_.push_back({598, 109, 233, 233});

  arctern::FaceTypeResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.m_vTypeResults[0].first<< ' ' << result.m_vTypeResults[0].second<< std::endl;
  }

  return 0;
}
