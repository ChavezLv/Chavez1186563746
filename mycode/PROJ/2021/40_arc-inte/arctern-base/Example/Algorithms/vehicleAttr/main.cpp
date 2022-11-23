/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/vehicleAttr/vehicleAttr.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::VehicleAttr recog;
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

  arctern::VehicleAttrRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  runParameter.rectsV_.push_back({32, 44, 292, 340}); //image: vehicle_feat_1.png
  runParameter.rectsV_.push_back({32, 44, 292, 340}); //image: vehicle_feat_1.png

  arctern::VehicleAttrResult result;
  ret = recog.Process(&runParameter,&result);

  auto attr = result.vehicleAttrs_[1];
  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "brand: " << "( " <<
        attr.vehicleBrand << ", " << attr.vehicleBrandScore << " )" 
        << std::endl;
    std::cout << "color: " << "( " <<
        attr.vehicleColor << ", " << attr.vehicleColorScore << " )"
        << std::endl;
    std::cout << "type: " << "( " <<
        attr.vehicleType << ", " << attr.vehicleTypeScore << " )"
        << std::endl;
  }
  return 0;
}
