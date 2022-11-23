/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.9
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_EXAMPLE_ALGORITHMS_FACETRACKER_DETECTRECOGSINK_H_
#define ARCTERN_EXAMPLE_ALGORITHMS_FACETRACKER_DETECTRECOGSINK_H_

#include "src/Algorithms/faceDetectTiny/faceDetectTiny.h"
#include "src/Algorithms/faceGluon/faceGluon.h"
#include <src/Algorithms/faceTracker/object_tracker.h>

class DetectRecogSink : public arctern::IDetectRecogSink {
 public:
  DetectRecogSink(std::string detectModel, std::string recogModel);
  std::vector<cv::Rect> OnDetect(cv::Mat &frameColor) override;
  std::vector<std::vector<uint8>> OnCalcFeatures(cv::Mat &frameColor, std::vector<cv::Rect> rects) override;

 private:
  arctern::FaceGluon recog_;
  arctern::FaceDetectTiny detect_;
};

#endif //ARCTERN_EXAMPLE_ALGORITHMS_FACETRACKER_DETECTRECOGSINK_H_
