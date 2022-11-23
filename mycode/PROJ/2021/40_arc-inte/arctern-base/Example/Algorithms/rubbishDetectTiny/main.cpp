#include "src/Algorithms/rubbishDetectTiny/rubbishDetectTiny.h"

#include "opencv2/opencv.hpp"

#include <iostream>
#include <istream> 
#include <cassert>

void ArcternBannetTest(){
  arctern::RubbishDetectTiny rubbishDetectTiny;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.thresh_ = 0.1;

  std::string imagePath = "/workspace/arctern-base/Example/Algorithms/rubbishDetectTiny/1.jpg"; 
  std::string modelPath = "/workspace/arctern/arctern-models/develop/unclassified/expose_rubbish-det-f32-d-0.1.0.bin";
  /* std::string imagePath = "/home/lvchaolin/arctern/arctern-base/Example/Algorithms/rubbishDetectTiny/1.jpg"; */ 
  /* std::string modelPath = "/home/lvchaolin/arctern/arctern-models/develop/unclassified/expose_rubbish-det-f32-d-0.1.0.bin"; */


  initParam.model_path = modelPath;

  auto ret = rubbishDetectTiny.Init(&initParam);
  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread(imagePath);

  /* cv::Mat imageRaw = cv::imread(imagePath); */
  /* cv::Mat imagePng = imageRaw.clone(); */
  /* cv::imwrite("/workspace/arctern-base/Example/Algorithms/rubbishDetectTiny/1.png", imagePng); */
  /* cv::Mat image = cv::imread("/workspace/arctern-base/Example/Algorithms/rubbishDetectTiny/1.png"); */


  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  img.step = image.step;

  std::cout << "img.width" << img.width << ", image.cols =" << image.cols << std::endl;
  std::cout << "img.height" << img.height << ", image.rows=" << image.rows << std::endl;

  arctern::RubbishDetectTinyRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  arctern::RubbishDetectTinyResult result;
  ret = rubbishDetectTiny.Process(&runParameter,&result);
  assert(ret == arctern::ErrorType::ERR_SUCCESS);
    
  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "get test result!" << std::endl;
    for(auto &info : result.detInfos_){
    //0 0.566661 [290 x 182 from (207, 157)]
        for_each(info.begin(), info.end(), [=](auto & one){
                    std::cout << "type = " << one.type\
                              << ", score = " << one.score\
                              << ", x = " << one.rect.x\
                              << ", y = " << one.rect.y\
                              << ", width = " << one.rect.width\
                              << ", height = " << one.rect.height; 

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
