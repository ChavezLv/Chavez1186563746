/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.03
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personBag/personBag.h"
#include <istream>

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::PersonBag recog;
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

  arctern::PersonBagRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  // runParameter.imageV_.push_back(img);

  /// face rect
  runParameter.rectsV_.push_back({175, 7, 332, 956});
  // runParameter.rectsV_.push_back({598, 109, 233, 233});

  ret = recog.Process(&runParameter,&result);

  auto impl = dynamic_cast<const arctern::PersonBagResult*>(result);
  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    for (auto& ele : impl->m_vClassifyResults)
    std::cout << ele.first << ' ' << ele.second << std::endl;
  }

  return 0;
}
