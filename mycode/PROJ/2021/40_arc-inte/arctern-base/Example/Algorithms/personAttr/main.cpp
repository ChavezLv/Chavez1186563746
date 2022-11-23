/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personAttr/personAttr.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::PersonAttr recog;
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

  arctern::PersonAttrRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  runParameter.rectsV_.push_back({175, 7, 332, 956}); //image: shortsleeve_11230.png
  runParameter.rectsV_.push_back({175, 7, 332, 956}); //image: shortsleeve_11230.png
  runParameter.personTruncationV_.push_back({0.000152843, 0.172508657}); //shortsleeve_11230.png
  runParameter.personTruncationV_.push_back({0.000152843, 0.172508657}); //shortsleeve_11230.png

  arctern::PersonAttrResult result;
  ret = recog.Process(&runParameter,&result);

  auto attr = result.personAttrs_[1];
  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "age: " << "( " <<
        static_cast<int>(attr.age) << ", " << attr.ageScore << " )" 
        << std::endl;
    std::cout << "hasbackpack: " << "( " <<
        static_cast<int>(attr.hasBackpack) << ", " << attr.hasBackpackScore << " )"
        << std::endl;
    std::cout << "hasCart: " << "( " <<
        static_cast<int>(attr.hasCart) << ", " << attr.hasCartScore << " )"
        << std::endl;
    std::cout << "isFrontHold: " << "( " <<
        static_cast<int>(attr.isFrontHold) << ", " << attr.isFrontHoldScore << " )"
        << std::endl;
    std::cout << "gender: " << "( " <<
        static_cast<int>(attr.gender) << ", " << attr.genderScore << " )"
        << std::endl;
    std::cout << "glasses: " << "( " <<
        static_cast<int>(attr.glasses) << ", " << attr.glassesScore << " )"
        << std::endl;
    std::cout << "hairColor: " << "( " <<
        static_cast<int>(attr.hairColor) << ", " << attr.hairColorScore << " )"
        << std::endl;
    std::cout << "hairLength: " << "( " <<
        static_cast<int>(attr.hairLength) << ", " << attr.hairLengthScore << " )"
        << std::endl;
    std::cout << "hasHandbag: " << "( " <<
        static_cast<int>(attr.hasHandbag) << ", " << attr.hasHandbagScore << " )"
        << std::endl;
    std::cout << "headwear: " << "( " <<
        static_cast<int>(attr.headwear) << ", " << attr.headwearScore << " )"
        << std::endl;
  }
  return 0;
}
