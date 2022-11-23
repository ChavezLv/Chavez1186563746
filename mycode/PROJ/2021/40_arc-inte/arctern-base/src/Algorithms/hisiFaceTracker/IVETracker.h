//
// Created by gh on 20-11-4.
//

#ifndef ARCTERN_IVETRACKER_H
#define ARCTERN_IVETRACKER_H

#include "include/arctern.h"

#include "opencv2/opencv.hpp"
#include "define.h"

namespace arctern {
class IVEObjectTracker;

class ARCTERN_EXPORT_API IVETracker {
 public:
  IVETracker(const arctern::ObjectTrackerParameter &param);
  ~IVETracker();

  std::vector<arctern::TrackerInfo> IVE_track(const cv::Mat &image);

  std::vector<arctern::TrackerInfo> IVE_update(const cv::Mat &image, const std::vector<cv::Rect> &rects);

 private:
  IVEObjectTracker *impl;
};
} // namespace arctern
#endif  // ARCTERN_IVETRACKER_H
