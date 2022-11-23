//
// Created by gh on 2021/1/13.
//

#include "rkfaceTracker.h"
#include "common/imageConvert.h"
#include "rk_of_object_tracker.h"

using arctern::RKTracker;
using arctern::ObjectTrackerParameter;
using arctern::RKOpticalFlowObjectTracker;
using arctern::TrackResult;

RKTracker::RKTracker(const arctern::ObjectTrackerParameter &param) :impl(new RKOpticalFlowObjectTracker(param)){

}

RKTracker::~RKTracker() {

}

namespace {
cv::Mat convertGrayMat(arctern::ArcternImage &image){
  cv::Mat mat;
  arctern::ArcternImage2Mat(image, mat);
  cv::Mat grayMat;
  arctern::Mat2GrayMat(mat, image.format, grayMat);
  return grayMat;
}
}

TrackResult RKTracker::RK_track(ArcternImage &image) {
  if (impl) {
    cv::Mat grayMat = convertGrayMat(image);
    return impl->track(grayMat);
  }

  return TrackResult();
}

TrackResult RKTracker::RK_update(ArcternImage &image, const std::vector<cv::Rect> &rects) {
  if (impl) {
    cv::Mat grayMat = convertGrayMat(image);
    return impl->update(grayMat, rects);
  }
  return TrackResult();
}