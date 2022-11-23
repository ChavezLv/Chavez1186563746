#pragma once

#include "define.h"
#include <memory>

namespace arctern {

//
// flow object tracker wrapper class
// specialization for RK3288/RK3399/MTK platform (limited cpu power)
// features: 
//      1. support tracker state save/load
//      2. optimize for face count
//      3. use default ref_cnt to avoid cv::Mat clone operation
//      
// limitations:
//      1. dont support tracking face that lost before and appear again soon
//      2. dont support track histories
//      3. face ids are allocated with 16 bit width, limited range is (0-65535)

class RKOpticalFlowObjectTracker {
public:
  RKOpticalFlowObjectTracker(RKOpticalFlowObjectTracker &tracker);
  RKOpticalFlowObjectTracker(const ObjectTrackerParameter &object_tracker_params);
  ~RKOpticalFlowObjectTracker();

  TrackResult track(cv::Mat &image);
  TrackResult update(cv::Mat &image, const std::vector<cv::Rect> &detection_results);

private:
  struct ExtSetting 
  {
      ObjectTrackerParameter track_params_;
      int overlap_thres_amend;
      int border_rate_thres;
      friend class State;
  };
  struct State;
  std::shared_ptr<ExtSetting> setting_;
  std::unique_ptr<State> state_;
  cv::Mat prev_frame_; ///< previous frame
}; // end for class OpticalFlowObjectTracker

} // end for namespace arctern_manager

