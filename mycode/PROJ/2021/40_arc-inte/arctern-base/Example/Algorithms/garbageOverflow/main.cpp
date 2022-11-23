#include "src/Algorithms/garbageOverflow/garbageOverflow.h"

#include "opencv2/opencv.hpp"

#include <iostream>
#include <istream> 
#include <cassert>

void ArcternGarbageOverflowTest(){
  arctern::GarbageOverflow GarbageOverflow;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.thresh_ = 0.5;

  std::string imagePath = "/workspace/arctern-base/Example/Algorithms/garbageOverflow/1.jpg"; 
  std::string modelPath = "/workspace/arctern-models/develop/unclassified/garbage_overflow-det-f32-d-0.1.0.bin";
  /* std::string imagePath = "/home/lvchaolin/arctern/arctern-base/Example/Algorithms/garbageOverflow/1.jpg"; */ 
  /* std::string modelPath = "/home/lvchaolin/arctern/arctern-models/develop/unclassified/garbage_overflow-det-f32-d-0.1.0.bin"; */

  initParam.model_path = modelPath;

  auto ret = GarbageOverflow.Init(&initParam);
  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat imageRaw = cv::imread(imagePath);
  cv::Mat image = imageRaw.clone();
  cv::imwrite("1.png", image);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  img.step = image.step;

  arctern::GarbageOverflowRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  arctern::GarbageOverflowResult result;
  ret = GarbageOverflow.Process(&runParameter,&result);
  assert(ret == arctern::ErrorType::ERR_SUCCESS);
    
  std::cout << "get test garbage result success!" << std::endl;

  for(auto &info : result.detInfos_){
      //0 0.870037 [41 x 44 from (78, 21)]
      int res = 1;
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
               cv::imwrite("/home/lvchaolin/arctern/arctern-base/Example/Algorithms/garbageOverflow/result" + to_string(res) + ".png", showImage);
               ++ res;
               std::cout << std::endl;
               });
      std::cout << std::endl;
  }

}

int main(){
    ArcternGarbageOverflowTest();
    return 0;
}
