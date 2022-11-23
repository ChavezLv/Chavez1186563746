/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceTracker/object_tracker/SingleTracker.h"

using cv::Mat;
using cv::Rect;

SingleTracker::SingleTracker() {
}

SingleTracker::~SingleTracker() {
}

void SingleTracker::ResetTracker(const Rect &rect, int nID, const cv::Mat &rMatCur, float confidence, int detect_plus) {
}
void SingleTracker::InitCandiFrameRect() {
}
void SingleTracker::SetNoUse() {
}
void SingleTracker::Process(const Mat &matCur, const Mat &matColor, int nFrameID) {
}
Rect *SingleTracker::GetCurPos() const {
  return NULL;
}
Rect *SingleTracker::GetPrevPos() const {
  return NULL;
}
