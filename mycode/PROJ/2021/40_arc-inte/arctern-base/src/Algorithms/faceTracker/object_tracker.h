/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_H_

#include <memory>
#include <vector>
#include "./define.h"

#include "object_tracker/MotTracker.h"

namespace arctern {
class ObjectTracker {
 public:
  explicit ObjectTracker(const ObjectTrackerParameter &object_tracker_params);

  void initDetectRecogSink(IDetectRecogSink *sink);

  TrackResult track(const cv::Mat &image);

#if CUDA_INTERFACE
  TrackResult track(const cv::cuda::GpuMat &image) {}  // TODO(no)
#endif
 private:
  float tracy_dist(std::vector<cv::Rect> rect_history);
  TrackResult counting(const std::vector<TrackerInfo> &track_info_list);

 private:
  ObjectTrackerParameter track_params_;
  std::shared_ptr<MotTracker> mot_tracker_;
  long frame_index_now_ = 0;     // NOLINT
  float frame_height_;
  std::vector<TrackerInfo> tracking_info_;
};
}  // namespace arctern
#endif        // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_H_
