/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.9
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "detectRecogSink.h"

DetectRecogSink::DetectRecogSink(std::string detectModel, std::string recogModel) {

  // std::string dir = "/home/gh/CLionProjects/arctern-models/develop/face/";
  arctern::InitParameter detInitParam;
  detInitParam.batchNum_ = 1;
  detInitParam.model_path = detectModel;  //dir + "face-det-tiny-f32-d-2.0.0.bin";
  auto ret = detect_.Init(&detInitParam);
  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  arctern::InitParameter recogInitParam;
  recogInitParam.batchNum_ = 1;
  recogInitParam.model_path = recogModel; // dir + "face-gluon-2.4.0.bin";
  ret = recog_.Init(&recogInitParam);

  assert(ret == arctern::ErrorType::ERR_SUCCESS);
}

std::vector<cv::Rect> DetectRecogSink::OnDetect(cv::Mat &frameColor) {
  std::vector<cv::Rect> rects;
  if (frameColor.empty()) {
    return rects;
  }
  ::arctern::ArcternImage img;
  img.width = frameColor.cols;
  img.height = frameColor.rows;
  img.imgdata.gdata = frameColor.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::RunParameter runParameter;
  runParameter.imageV_.push_back(img);


  auto result = detect_.Process(&runParameter);
  auto *detResult = dynamic_cast<const arctern::FaceDetectTinyResult *>(result);

  if (detResult->errorType_ != arctern::ErrorType::ERR_SUCCESS) {
    return rects;
  }

  /// only just one picture
  int size = detResult->detInfos_[0].size();
  for (int i = 0; i < size; i++) {
    auto rect = detResult->detInfos_[0][i].rect;

    rects.emplace_back(cv::Rect(rect.x, rect.y, rect.width, rect.height));
  }
  return rects;
}

std::vector<std::vector<uint8>> DetectRecogSink::OnCalcFeatures(cv::Mat &frameColor, std::vector<cv::Rect> rects) {
  std::vector<std::vector<uint8>> output;
  if(frameColor.empty() || rects.empty()) {
    return output;
  }

  ::arctern::ArcternImage img;
  img.width = frameColor.cols;
  img.height = frameColor.rows;
  img.imgdata.gdata = frameColor.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

  arctern::FaceGluonRunParameter runParameter;

  for (auto rect : rects) {
    runParameter.imageV_.push_back(img);
    arctern::ArcternRect faceRect;
    faceRect.x = rect.x;
    faceRect.y = rect.y;
    faceRect.width = rect.width;
    faceRect.height = rect.height;

    runParameter.rectsV_.push_back(faceRect);
  }

  auto result = recog_.Process(&runParameter);
  auto feats = dynamic_cast<const arctern::FaceGluonResult *>(result);


  if (feats->errorType_ == arctern::ErrorType::ERR_SUCCESS) {
    output = feats->features_;
  }

  return output;
}


