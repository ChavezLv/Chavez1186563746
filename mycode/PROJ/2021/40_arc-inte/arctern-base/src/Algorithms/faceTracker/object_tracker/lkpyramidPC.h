/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_LKPYRAMIDPC_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_LKPYRAMIDPC_H_
#include "./utilsfunction.h"

namespace ifcv {
namespace detail {

typedef short deriv_type;  // NOLINT

struct LKTrackerInvoker {
  LKTrackerInvoker(const cv::Mat &_prevImg, const cv::Mat &_prevDeriv, const cv::Mat &_nextImg,
                   const cv::Point2f *_prevPts, cv::Point2f *_nextPts,
                   uchar *_status, float *_err,
                   cv::Size _winSize, cv::TermCriteria _criteria,
                   int _level, int _maxLevel, int _flags, float _minEigThreshold);

  void runLK(int nPoint);  // @zzf added

  const cv::Mat *prevImg;
  const cv::Mat *nextImg;
  const cv::Mat *prevDeriv;
  const cv::Point2f *prevPts;
  cv::Point2f *nextPts;
  uchar *status;
  float *err;
  cv::Size winSize;
  cv::TermCriteria criteria;
  int level;
  int maxLevel;
  int flags;
  float minEigThreshold;
};

}  // namespace detail

}  // namespace ifcv
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_LKPYRAMIDPC_H_


