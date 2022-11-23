
#include "src/Algorithms/licenseLandmark/licenseLandmark.h"
#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::LicenseLandmark recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/data/leizhiming/arctern-models/develop/vehicle/license-landmark-d-1.0.0.bin";
  auto ret = recog.Init(&initParam);
  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread("/data/leizhiming/arctern-metadata/ModelsVerifyData/data/licenseLandmark/000.jpg");

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.step = image.step;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::LicenseLandmarkRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  arctern::LicenseLandmarkResult result;
  ret = recog.Process(&runParameter,&result);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    for(int i=0; i < result.LicenseLandmarks_.size();i++){
      for(int j=0;j<result.LicenseLandmarks_[i].size();j++)
        std::cout << "result : " << result.LicenseLandmarks_[i][j].x <<" "<<result.LicenseLandmarks_[i][j].y << std::endl;
    }

  }
  return 0;
}
