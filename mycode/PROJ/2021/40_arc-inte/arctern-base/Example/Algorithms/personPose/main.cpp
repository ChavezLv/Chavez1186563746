/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.03
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personPose/personPose.h"
#include <istream>

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::PersonPose recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/home/gh/CLionProjects/arctern-models/develop/body/pose-predictor-f32-d-0.0.3.bin";
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread("/home/gh/CLionProjects/arctern-metadata/ModelsVerifyData/data/predict_pose/3.png");

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::PersonPoseRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  // runParameter.imageV_.push_back(img);

  /// face rect
  runParameter.rectsV_.push_back({145, 473, 131, 261});
  // runParameter.rectsV_.push_back({598, 109, 233, 233});

  arctern::PersonPoseResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    for (auto& ele : result.results_) {
      for(auto point : ele) {
        std::cout<<point.first.x<<" "<<point.second<<std::endl;
      }

    }
    //std::cout << ele.first << ' ' << ele.second << std::endl;
  }

  return 0;
}
