/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceGlass/faceGlass.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::FaceGlass recog;
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

  arctern::FaceGlassRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  runParameter.rectsV_.push_back({597, 105, 223, 223});

  arctern::FaceGlassResult result;
  ret = recog.Process(&runParameter,&result);

   assert(ret == arctern::ErrorType::ERR_SUCCESS);

  // just one picture
  std::cout << result.FaceGlasss_[0].statusScore << std::endl;

  return 0;
}
