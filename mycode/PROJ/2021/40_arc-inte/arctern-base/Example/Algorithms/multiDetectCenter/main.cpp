/****************************************************************************
 *  Filename:       initparameter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.27
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/multiDetectCenter/multiDetectCenter.h"
#include "opencv2/opencv.hpp"
#include <cassert>
void MultiDetectCenterTest(const std::string &model_path,const std::string &img_path,int batchSize = 1){
  std::cout << "==========(multi detect center inference starting)==========" << std::endl;
  arctern::InitParameter init_parameter;
  init_parameter.batchNum_ = batchSize;
  init_parameter.model_path = model_path;
  init_parameter.thresh_ = 0.3;
 // init_parameter.typeBits_ = arctern::ARCTERN_INT8;
  arctern::MultiDetectCenter multi_detect_center;
  auto init_result = multi_detect_center.Init(&init_parameter);
  assert(init_result == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat multi_detect_mat = cv::imread(img_path);
  arctern::ArcternImage img;
  img.width = multi_detect_mat.cols;
  img.height = multi_detect_mat.rows;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  img.imgdata.gdata = static_cast<void*>(multi_detect_mat.data);

  img.type = arctern::CPU;


  arctern::RunParameter multi_detect_run_param;

  for(int i=0;i<batchSize;i++){
    multi_detect_run_param.imageV_.push_back(img);
  }

  arctern::MultiDetectCenterResult result;
  auto ret = multi_detect_center.Process(&multi_detect_run_param,&result);

  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  std::cout <<"pictures:"<<result.detInfos_.size()<<std::endl;
  for(const auto &infos:result.detInfos_){
    std::cout <<"[ ";
    for(const auto &item:infos){
      std::cout <<"class:"<<static_cast<int>(item.type)<<" rect:("<<item.rect.x<<","<<item.rect.y<<","<<item.rect.width<<","<<item.rect.height<<
                ") score:"<<item.score <<" quality:"<<item.quality <<std::endl;
    }
    std::cout <<"] classes(1 picture):"<<infos.size()<<endl;
  }
  std::cout << "==========(multi detect center inference finished)==========" << std::endl <<std::endl;
}

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {

/*  MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-cached-i8-q1-d-0.0.6.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        1);
  MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-cached-i8-q1-d-0.0.6.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        1);*/

  MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-cached-i8-q1-d-0.0.4.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        4);
  MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-cached-i8-q1-d-0.0.4.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        4);
  /*MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-f32-d-0.0.6.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        4);
  MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-f32-d-0.0.6.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        4);*/

/*  MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-cached-i8-q1-d-0.0.4.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        4);*/

  /*MultiDetectCenterTest("/data/leizhiming/arctern-models/develop/unclassified/body-face-head-detector-center-combined-f32-d-0.0.6.bin",
                        "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/detect_multi_cat_center/0.png",
                        4);*/
  return 0;
}
