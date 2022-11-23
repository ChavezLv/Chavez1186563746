/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_GLUONFEATURE_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_GLUONFEATURE_H_

#include <vector>
#include <memory>
#include "opencv2/opencv.hpp"

std::vector<float> CalcFeature(cv::Mat &frame_color, cv::Rect rect);

float CalcSimilarity(const std::vector<float> *feat1, const std::vector<float> *feat2);

std::vector<float> CombineFeature(const std::vector<float> &feat_new, const std::vector<float> &feat_old, float aratio);
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_GLUONFEATURE_H_


