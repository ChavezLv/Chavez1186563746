/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceQuality/faceQuality.h"
#include "opencv2/opencv.hpp"
#include <cassert>
#include <thread>
void runFaceQuality(){
  arctern::FaceQuality recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/data/leizhiming/arctern-models/develop/face/face-quality-d-1.0.3.bin";
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread("/data/leizhiming/arctern-metadata/ModelsVerifyData/data/predict_face_quality/child1.png");

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::FaceQualityRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  runParameter.rectsV_.push_back({157, 292, 496, 496});

  arctern::FaceQualityResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.faceQuality_[0] << std::endl;
  }

}

//void runFacePose(){
//  arctern::FacePose recog;
//  arctern::InitParameter initParam;
//  initParam.batchNum_ = 1;
//  initParam.model_path = "/data/leizhiming/arctern-models/develop/face/face-pose-cls-d-1.0.0.bin";
//  auto ret = recog.Init(&initParam);
//
//
//  assert(ret == arctern::ErrorType::ERR_SUCCESS);
//
//  cv::Mat image = cv::imread("/data/leizhiming/arctern-metadata/ModelsVerifyData/data/predict_face_quality/child1.png");
//
//  ::arctern::ArcternImage img;
//  img.width = image.cols;
//  img.height = image.rows;
//  img.imgdata.gdata = image.data;
//  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
//
//  arctern::FacePoseRunParameter runParameter;
//  runParameter.imageV_.push_back(img);
//
//  /// face rect, need change
//  runParameter.rectsV_.push_back({157, 292, 496, 496});
//
//  arctern::FacePoseResult result;
//  ret = recog.Process(&runParameter,&result);
//
//  if (ret == arctern::ErrorType::ERR_SUCCESS) {
//    std::cout << result.facePose_[0].pitch << std::endl;
//  }
//}

int main(int argc, char **argv) {
  vector<std::thread> vThreads;
  for(int i=0;i<5;i++){
  //  vThreads.emplace_back(std::thread(runFacePose));
    vThreads.emplace_back(std::thread(runFaceQuality));
  }
  for(int i=0;i<10;i++){
    vThreads[i].join();
  }
  return 0;
}
