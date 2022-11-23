/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_OVERLAP_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_OVERLAP_H_

#include <opencv2/opencv.hpp>

class seRectPair {
 private:
  class seRect {
   public:
    float startX;
    float startY;
    float endX;
    float endY;
    seRect() {}
    explicit seRect(cv::Rect r);
    ~seRect() {}
  };
  seRect r1;
  seRect r2;
  seRect r_intersect;
  float union_area;
  float PairIntersect();
  float PairUnion();

 public:
  seRectPair() {}
  seRectPair(cv::Rect, cv::Rect r2);
  ~seRectPair() {}
  float getOverlapRatio();  // denominator is the two rects' union area
  float getSelfOverlapRatio();  // denominator is the first rect's area
  float getMaxOverlapRatio();  // denominator is the litter rect's area
};
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_OVERLAP_H_
