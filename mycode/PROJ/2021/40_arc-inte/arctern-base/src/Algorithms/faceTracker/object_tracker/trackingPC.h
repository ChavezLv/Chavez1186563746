/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_TRACKINGPC_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_TRACKINGPC_H_

#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

extern std::fstream RInfo_txt;

#ifdef __cplusplus
extern "C" {
#endif

/************************************ optical flow ***************************************/

#define  IFCV_LKFLOW_PYR_A_READY       1
#define  IFCV_LKFLOW_PYR_B_READY       2
#define  IFCV_LKFLOW_INITIAL_GUESSES   4
#define  IFCV_LKFLOW_GET_MIN_EIGENVALS 8

/* It is Lucas & Kanade method, modified to use pyramids.
   Also it does several iterations to get optical flow for
   every point at every pyramid level.
   Calculates optical flow between two images for certain set of points (i.e.
   it is a "sparse" optical flow, which is opposite to the previous 3 methods) */
#ifdef __cplusplus
}

// @zzf namespace used to make sure the only meaning of the variable name, function name, calss name;
namespace ifcv {
enum {
  OPTFLOW_USE_INITIAL_FLOW = IFCV_LKFLOW_INITIAL_GUESSES,
  OPTFLOW_LK_GET_MIN_EIGENVALS = IFCV_LKFLOW_GET_MIN_EIGENVALS,
};
// @zzf The above expression equal to "define OPTFLOW_USE_INITIAL_FLOW = CV_LKFLOW_INITIAL_GUESSES"
// "define OPTFLOW_LK_GET_MIN_EIGENVALS = CV_LKFLOW_GET_MIN_EIGENVALS"

// @zzf calculate the max level of an image
int calMaxLevel(cv::Size rectSize, cv::Size winSize, int maxLevel);

//! constructs a pyramid which can be used as input for calcOpticalFlowPyrLK
CV_EXPORTS_W int buildOpticalFlowPyramidPC(cv::InputArray img, cv::OutputArrayOfArrays pyramid,
                                           cv::Size winSize, int maxLevel, int pyrBorder = cv::BORDER_REFLECT_101);

//! computes sparse optical flow using multi-scale Lucas-Kanade algorithm
CV_EXPORTS_W void calcOpticalFlowPyrLKPC(cv::InputArray prevImg, cv::InputArray nextImg,
                                         cv::InputArray prevPts, CV_OUT cv::InputOutputArray nextPts,
                                         cv::OutputArray status, cv::OutputArray err, cv::Rect targetRect,
                                         cv::Size winSize = cv::Size(21, 21), int maxLevel = 3,
                                         cv::TermCriteria criteria = cv::TermCriteria(
                                             cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 0.01),
                                         int flags = 0, double minEigThreshold = 1e-4);
}  // namespace ifcv
#endif
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_TRACKINGPC_H_
