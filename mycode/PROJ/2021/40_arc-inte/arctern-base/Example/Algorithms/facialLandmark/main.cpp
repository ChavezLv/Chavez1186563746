/****************************************************************************
 *  Filename:       initparameter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/facialLandmark/facialLandmark.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::FacialLandmark landmark;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = argv[1];
  auto ret = landmark.Init(&initParam);
  if(ret != arctern::ErrorType::ERR_SUCCESS){
    exit(-1);
  }

  cv::Mat image = cv::imread(argv[2]);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::FacialLandmarkRunParameter runParameter;
  runParameter.imageV_.push_back(img);
 // runParameter.imageV_.push_back(img);

  /// face rect
  runParameter.rectsV_.push_back({107, 227, 599, 599});
 // runParameter.rectsV_.push_back({598, 109, 233, 233});

  arctern::FacialLandmarkResult result;
  ret = landmark.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    /// pictures num
    int len = runParameter.imageV_.size();

    for (int i = 0; i < len; i++) {
      cv::Mat showImage = image.clone();

      int num = result.landmarks_[i].size();

      // std::cout << " num = " << num << std::endl;
      for (int pointId = 0; pointId < num; pointId++) {
        cv::Point point(result.landmarks_[i][pointId].x,
                        result.landmarks_[i][pointId].y);
        cv::circle(showImage, point, 2, cv::Scalar(0, 0, 255),2);

        std::cout<<pointId<<"------->"<<point<<std::endl;
      }
      cv::imwrite("output.jpg", showImage);
    }
  }
  return 0;
}
