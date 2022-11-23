//
// Created by gh on 20-11-4.
//

#include "IVETracker.h"
#include "hisi_object_tracker/IVE_object_tracker.h"
#include <memory>
using arctern::ObjectTrackerParameter;
using arctern::TrackerInfo;
using arctern::IVEObjectTracker;
using arctern::IVETracker;

IVETracker::IVETracker(const arctern::ObjectTrackerParameter &param) {
  impl = new IVEObjectTracker(param, param.channel_idx);
}

IVETracker::~IVETracker() {
  if (nullptr != impl) {
    delete impl;
    impl = nullptr;
  }
}

std::vector<arctern::TrackerInfo> IVETracker::IVE_track(const cv::Mat &image) {
  if (nullptr != impl) {
    return impl->non_detection_track(image);
  }
  return std::vector<TrackerInfo>();
}

std::vector<arctern::TrackerInfo> IVETracker::IVE_update(const cv::Mat &image, const std::vector<cv::Rect> &rects) {
  if (nullptr != impl) {
    return impl->non_detection_update(image, rects);
  }
  return std::vector<TrackerInfo>();
}
