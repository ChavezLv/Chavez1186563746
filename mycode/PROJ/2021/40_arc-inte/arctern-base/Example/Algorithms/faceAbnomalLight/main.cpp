/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <opencv2/imgcodecs.hpp>
#include "src/Algorithms/faceAbnomalLight/faceAbnomalLight.h"
#define ARCTERN_PROJECT_PATH "C:/Users/Admin/Documents/arctern-base"
using namespace std;
#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
    cout << "start"<<endl;
  arctern::FaceAbnomalLight recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  //std::string path =   ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-abnomal-light-classifier-d-0.0.2.bin");
  initParam.model_path = argv[1];
  cout <<"1" <<endl;
  auto ret = recog.Init(&initParam);

  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread(argv[2]);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::FaceAbnomalLightRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  /// face rect, need change
  runParameter.rectsV_.push_back({597, 105, 223, 223});

  arctern::FaceAbnomalLightResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.faceAbnomalLights_[0].score << std::endl;
  }

  return 0;
}
