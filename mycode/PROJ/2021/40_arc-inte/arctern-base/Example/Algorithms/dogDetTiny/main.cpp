/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/dogDetTiny/dogDetTiny.h"
#include <istream>

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
    std::cout<<"kuanghongshen test!!!"<<std::endl;
  arctern::dogDetTiny recog;
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

  arctern::RunParameter runParameter;
  runParameter.imageV_.push_back(img);

  arctern::dogDetTinyResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
   // std::cout << impl->ageGenders_[0].age << std::endl;
  }

  return 0;
}
