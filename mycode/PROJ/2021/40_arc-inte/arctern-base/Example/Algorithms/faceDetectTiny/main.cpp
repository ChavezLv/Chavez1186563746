#include "src/Algorithms/faceDetectTiny/faceDetectTiny.h"
#include "opencv2/opencv.hpp"
int main(int argc, char **argv) {
  arctern::FaceDetectTiny det;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = argv[1];
  initParam.thresh_ = 0.6;
  auto ret = det.Init(&initParam);
  if (ret != arctern::ErrorType::ERR_SUCCESS) {
    exit(-1);
  }


  cv::Mat image = cv::imread(argv[2]);

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::RunParameter runParameter;
  runParameter.imageV_.push_back(img);
  runParameter.imageV_.push_back(img);
  runParameter.imageV_.push_back(img);

//  auto result = det.Process(&runParameter);
//
//  auto *detResult = dynamic_cast<const arctern::FaceDetectTinyResult *>(result);
//
//  if (detResult->errorType_ == arctern::ErrorType::ERR_SUCCESS) {
//    // now just only one image
//    int pictureNum = detResult->detInfos_.size();
//    std::cout << " picture num = " << pictureNum << std::endl;
//
//    for (int pId = 0; pId < pictureNum; pId++) {
//      cv::Mat showImage = image.clone();
//
//      int num = detResult->detInfos_[pId].size();
//
//      std::cout << " rect num = " << num << std::endl;
//
//      for (int i = 0; i < num; i++) {
//        cv::Rect rect(detResult->detInfos_[pId][i].rect.x,
//                      detResult->detInfos_[pId][i].rect.y,
//                      detResult->detInfos_[pId][i].rect.width,
//                      detResult->detInfos_[pId][i].rect.height);
//        std::cout << rect << std::endl;
//        std::cout << "score = " << detResult->detInfos_[pId][i].score << std::endl;
//
//        cv::rectangle(showImage, rect, cv::Scalar(0, 0, 255), 1);
//      }
//      cv::imshow("image", showImage);
//      cv::waitKey(-1);
//    }
//  }
  return 0;
}
