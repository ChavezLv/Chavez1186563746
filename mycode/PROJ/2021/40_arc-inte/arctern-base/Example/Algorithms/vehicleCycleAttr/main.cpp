/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/vehicleCycleAttr/vehicleCycleAttr.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::VehicleCycleAttr recog;
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

  arctern::VehicleCycleAttrRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  runParameter.rectsV_.push_back({1, 1, 1000, 1000}); //image: 2.png
  runParameter.rectsV_.push_back({1, 1, 1000, 1000}); //image: 2.png
  //runParameter.rectsV_.push_back({75, 474, 447, 920}); //image: 1.png
  //runParameter.rectsV_.push_back({75, 474, 447, 920}); //image: 1.png

  arctern::VehicleCycleAttrResult result;
  ret = recog.Process(&runParameter,&result);

  auto attr = result.vehicleCycleAttrs_[1];
  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "type: " << "( " <<
        static_cast<int>(attr.vehicleNoMotorType) << ", " 
        << attr.vehicleNoMotorTypeScore << " )" 
        << std::endl;
    std::cout << "hasUmbrella: " << "( " <<
        static_cast<int>(attr.hasUmbrella) << ", " << attr.hasUmbrellaScore << " )"
        << std::endl;
    std::cout << "isCycling: " << "( " <<
        static_cast<int>(attr.isCycling) << ", " << attr.isCyclingScore << " )"
        << std::endl;
    std::cout << "isMultiPerson: " << "( " <<
        static_cast<int>(attr.isMultiPerson) << ", " << attr.isMultiPersonScore << " )"
        << std::endl;
    std::cout << "orientationType: " << "( " <<
        static_cast<int>(attr.orientationType) << ", " 
        << attr.orientationTypeScore << " )" 
        << std::endl;
  }
  return 0;
}
