/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceTracker/faceTracker.h"
#include "./object_tracker.h"
#include "common/imageConvert.h"

using arctern::FaceTracker;

FaceTracker::FaceTracker(const ObjectTrackerParameter &tracker_params) {
  impl_ = new ObjectTracker(tracker_params);
}

FaceTracker::~FaceTracker() {
  delete impl_;
}

void FaceTracker::initDetectRecogSink(IDetectRecogSink *sink) {
  impl_->initDetectRecogSink(sink);
}

arctern::TrackResult FaceTracker::trackFace(arctern::ArcternImage image) {
  cv::Mat mat;
  arctern::ArcternImage2Mat(image, mat);

  cv::Mat bgrMat;
  arctern::Mat2BgrMat(mat, image.format, bgrMat);

  return impl_->track(bgrMat);
}
