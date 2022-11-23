/****************************************************************************
 *  Filename:       initparameter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceDetectRTN/faceDetectRTN.h"

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::FaceDetectRTN det;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = argv[1];
  initParam.typeBits_ = arctern::ARCTERN_INT8;
  initParam.thresh_ = 0.45;
  auto ret = det.Init(&initParam);
  //std::cout << ret << std::endl;

  cv::Mat image = cv::imread(argv[2]);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.step = image.step;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::FaceDetectRTNRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  //runParameter.imageV_.push_back(img);

  arctern::FaceDetectRTNResult result;
  ret = det.Process(&runParameter,&result);

  const cv::Scalar color(0, 255, 0);
  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    // now just only one image
    int num = result.detInfos_.size();
    for (int i = 0; i < num; i++) {
      cv::Rect rect(result.detInfos_[0][i].rect.x, result.detInfos_[0][i].rect.y,
                    result.detInfos_[0][i].rect.width, result.detInfos_[0][i].rect.height);
      cv::rectangle(image, rect, cv::Scalar(0, 0, 255), 1);
      cv::circle(image, cv::Point(result.detInfos_[0][i].landmarks[0].x,
                                  result.detInfos_[0][i].landmarks[0].y), 1, color);
      cv::circle(image, cv::Point(result.detInfos_[0][i].landmarks[1].x,
                                  result.detInfos_[0][i].landmarks[1].y), 1, color);
      cv::circle(image, cv::Point(result.detInfos_[0][i].landmarks[2].x,
                                  result.detInfos_[0][i].landmarks[2].y), 1, color);
      cv::circle(image, cv::Point(result.detInfos_[0][i].landmarks[3].x,
                                  result.detInfos_[0][i].landmarks[3].y), 1, color);
      cv::circle(image, cv::Point(result.detInfos_[0][i].landmarks[4].x,
                                  result.detInfos_[0][i].landmarks[4].y), 1, color);
    }
    cv::imwrite("/data/leizhiming/int8.jpg",image);
  }
  return 0;
}
