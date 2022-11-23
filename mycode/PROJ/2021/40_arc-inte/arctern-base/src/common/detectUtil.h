/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_DETECTUTIL_H_
#define SRC_COMMON_DETECTUTIL_H_

#include <vector>
#include "include/arctern.h"
#include "opencv2/opencv.hpp"

namespace arctern {

typedef struct {
  float cx;
  float cy;
  float s_kx;
  float s_ky;
} prior_box;

/**
 * facial landmark,5 points
 * @param leye left eye center location
 * @param reye right eye center location
 * @param nose nose center location
 * @param lmouth left mouth corner location
 * @param rmouth right mouth corner location
 */
typedef struct {
  cv::Point2f leye;
  cv::Point2f reye;
  cv::Point2f nose;
  cv::Point2f lmouth;
  cv::Point2f rmouth;
} landmark_point;

/**
 * Bounding box structure used for express a face area
 * @param  xx1 x coordinates of top-left point in face box
 * @param  yy1 y coordinates of top-left point in face box
 * @param  xx2 x coordinates of bottom-right point in face box
 * @param  yy2 y coordinates of bottom-right point in face box
 * @param  score bouding box's score, which represent the likelihood of face
 * @param  dxx1 regression value of x coordinates of top-left point
 * @param  dyy1 regression value of y coordinates of top-left point
 * @param  dxx2 regression value of x coordinates of bottom-right point
 * @param  dyy2 regression value of y coordinates of bottom-right point
 * @param  lmd  facial landmark, five points
 */
struct Bbox {
  Bbox(int xx1, int yy1, int xx2, int yy2, float box_score, float dxx1 = 0.,
       float dyy1 = 0., float dxx2 = 0., float dyy2 = 0.)
      : x1(xx1),
        y1(yy1),
        x2(xx2),
        y2(yy2),
        score(box_score),
        dx1(dxx1),
        dy1(dyy1),
        dx2(dxx2),
        dy2(dyy2) {}

  float x1, y1, x2, y2;
  float score;
  float dx1, dy1, dx2, dy2;
  int label;
  landmark_point lmd;
  float quality;
};

typedef std::vector<Bbox> Vbbox;

/// \brief calc iou of both rect
float box_iou(ArcternRect a, ArcternRect b);

/// \brief transform the rect to square
void ConvertSquare(Vbbox &bbox, float exth_scale, float extw_scale);
}  // namespace arctern
#endif  // SRC_COMMON_DETECTUTIL_H_
