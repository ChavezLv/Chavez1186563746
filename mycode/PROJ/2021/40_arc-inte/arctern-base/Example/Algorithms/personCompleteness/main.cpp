/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.04
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/personCompleteness/personCompleteness.h"
#include <cassert>
#include "opencv2/opencv.hpp"
void Test(const std::string& model_path,const std::string& img_path){
  arctern::PersonCompleteness recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 2;
  initParam.model_path = model_path;
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread(img_path);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  img.type = arctern::CPU;


  arctern::PersonCompletenessRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  //runParameter.rectsV_.push_back({215, 3, 273, 1032}); //image: shortsleeve_6210.png
  //runParameter.rectsV_.push_back({67, 0, 300, 527}); //image: person_attributes.png
  runParameter.rectsV_.push_back({215, 3, 273, 1032}); //image: handbag_1853.png
  runParameter.rectsV_.push_back({215, 3, 273, 1032}); //image: handbag_1853.png

  arctern::PersonCompletenessResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    for(int i=0;i<result.personCompleteness_.size();i++){
      std::cout << "truncation: " << "( " <<
                result.personCompleteness_[i].truncation.at(0) << ", " <<
                result.personCompleteness_[i].truncation.at(1) << " )"
                << std::endl;
      std::cout << "visible_parts: "<<std::endl;
      for (auto vp : result.personCompleteness_[i].visibleParts) {
        std::cout << vp << ", ";
      }
      std::cout <<std::endl;
      std::cout << "soft mask : "<<std::endl;
      for (auto sm : result.personCompleteness_[i].softMasks) {
        for (auto sm1 : sm) {
          std::cout << sm1 << ", ";
        }
      }
      std::cout <<std::endl;
    }
  }
}

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  Test("/data/leizhiming/arctern-models/develop/body/person-completeness-0.0.0.bin",
       "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/predict_person_completeness/shortsleeve_6210.png");
  Test("/data/leizhiming/arctern-models/develop/body/person-completeness-0.0.0.bin",
       "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/predict_person_completeness/shortsleeve_6210.png");
  return 0;
}
