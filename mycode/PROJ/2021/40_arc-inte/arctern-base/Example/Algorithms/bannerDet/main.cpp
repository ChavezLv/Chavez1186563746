#include "src/Algorithms/bannerDet/bannerDet.h"

#include "opencv2/opencv.hpp"

#include <iostream>
#include <istream> 
#include <cassert>

void ArcternBannetTest(){
    ////0 0.98769 [597 x 208 from (1324, 264)]
    //std::string rootPath = "...";
    //std::vector<std::string> vPaths;
    //vPaths.push_back(rootPath + "1.jpg");
    //
    //std::vector<cv::Mat> vImages;
    //for(int i=0;i<vPaths.size();i++){
    //    auto img = cv::imread(vPaths[i]);
    //    vImages.push_back(img);
    //}
  arctern::BannerDet bannerDet;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.thresh_ = 0.1;

  std::string rootPath = "/workspace/arctern-base/Example/Algorithms/bannerDet/1.jpg"; 
  std::string modelPath = "/workspace/arctern-models/develop/unclassified/banner-det-f32-d-0.1.0.bin";
  /* std::string rootPath = "/home/lvchaolin/arctern/arctern-base/Example/Algorithms/bannerDet/1.jpg"; */ 
  /* std::string modelPath = "/home/lvchaolin/arctern/arctern-models/develop/unclassified/banner-det-f32-d-0.1.0.bin"; */

  initParam.model_path = modelPath;

  auto ret = bannerDet.Init(&initParam);
  assert(ret == arctern::ErrorType::ERR_SUCCESS);


  cv::Mat imageRaw = cv::imread(rootPath);
  cv::Mat image = imageRaw.clone();
  cv::imwrite("1.png", image);
  /* cv::Mat image = cv::imread(rootPath, cv::IMREAD_COLOR); */

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  img.step = image.step;

  arctern::BannerDetRunParameter runParameter;
  runParameter.imageV_.push_back(img);


  arctern::BannerDetResult result;
  ret = bannerDet.Process(&runParameter,&result);
  /* assert(ret == arctern::ErrorType::ERR_SUCCESS); */
    

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
      std::cout << "get test bannerDet result success!" << std::endl;
      //0 0.98769 [597 x 208 from (1324, 264)]
      for(auto &info : result.detInfos_){
          for_each(info.begin(), info.end(), [&](auto & one){
                   std::cout << "type = " << one.type\
                   << ", score = " << one.score\
                   << ", x = " << one.rect.x\
                   << ", y = " << one.rect.y\
                   << ", width = " << one.rect.width\
                   << ", height = " << one.rect.height; 
                   cv::Mat showImage = image.clone();
                   cv::Rect rect(one.rect.x, one.rect.y, one.rect.width, one.rect.height);
                   cv::rectangle(showImage, rect, cv::Scalar(0, 0, 255), 1);
                   cv::imwrite("/home/lvchaolin/arctern/arctern-base/Example/Algorithms/bannerDet/result.png", showImage);
                   std::cout << std::endl;
                   });
          std::cout << std::endl;
      }
  }else{
      std::cout << "ERROR ret! " << ret << std::endl;
  }
}

int main(){
    ArcternBannetTest();
    return 0;
}
