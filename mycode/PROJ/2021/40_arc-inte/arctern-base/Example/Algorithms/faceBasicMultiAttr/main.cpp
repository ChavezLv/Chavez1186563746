/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.25
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceBasicMultiAttr/faceBasicMultiAttr.h"
#include "opencv2/opencv.hpp"
#include <cassert>
#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::FaceBasicMultiAttr recog;
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

  arctern::FaceBasicMultiAttrRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  //runParameter.rectsV_.push_back({0, 0, img.width, img.height});
  runParameter.rectsV_.push_back({19, 67, 67, 69});

  arctern::FaceBasicMultiAttrResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    auto res1 = result.faceBasicMultiAttrs_.at(0);
    std::cout << "gender:"<< static_cast<int>(res1.gender)<<", "<<res1.genderScore << std::endl;
    std::cout << "age:"<< static_cast<int>(res1.age)<<", "<<res1.ageScore << std::endl;
    std::cout << "glasses:"<< static_cast<int>(res1.glasses)<<", "<<res1.glassesScore << std::endl;
    std::cout << "mask:"<< static_cast<int>(res1.mask)<<", "<<res1.maskScore << std::endl;
    std::cout << "headwear:"<< static_cast<int>(res1.headwear)<<", "<<res1.headwearScore << std::endl;
  }
  return 0;
}
