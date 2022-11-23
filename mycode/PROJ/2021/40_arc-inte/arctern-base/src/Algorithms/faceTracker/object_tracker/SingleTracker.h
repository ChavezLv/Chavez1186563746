/// @brief basic class for single obeject tracker
/// @author Zhang Zhaofeng
//  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
/// @date 2018.8.23

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_SINGLETRACKER_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_SINGLETRACKER_H_

#include <opencv2/opencv.hpp>
#include <vector>

class SingleTracker {
 public:
  SingleTracker();
  ~SingleTracker();
  virtual void ResetTracker(const cv::Rect &rect, int nID, const cv::Mat &rMatCur, float confidence, int detect_plus);
  virtual void InitCandiFrameRect();
  virtual void SetNoUse();
  virtual void Process(const cv::Mat &matCur, const cv::Mat &matColor, int nFrameID);
  virtual cv::Rect *GetCurPos() const;
  virtual cv::Rect *GetPrevPos() const;
};

#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_SINGLETRACKER_H_
