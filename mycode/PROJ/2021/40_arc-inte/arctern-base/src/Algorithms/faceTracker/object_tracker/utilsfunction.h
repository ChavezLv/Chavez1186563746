/*
* @file utilsfunction.h
* @date 2016-03-08
* @brief some utilis function in tracking.
* @author ZhangZhaofeng(zhang.zhaofeng@intellif.com)
*
* Copyright (c) 2016, Shenzhen Intellifusion Technologies Co., Ltd.
*/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_UTILSFUNCTION_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_UTILSFUNCTION_H_

#include <vector>
#include "opencv2/opencv.hpp"

float RectangleIntersect(cv::Rect *rect1, cv::Rect *rect2);
bool BoxPosFilter(cv::Rect *rect, int nWidth, int nHeight, int nMargin);
bool BoxSizeFilter(cv::Rect *rect, int nDim);

bool Rule1(int R, int G, int B);
int getSkin(cv::Mat const &src);

// 人脸框转opencv rect
// void ifrt2Rect(cv::Rect *tracker_rect, if_rect_t *detect_rect, int image_width, int image_height);

// 获取框的中心
void get_rect_center(cv::Rect *tracker_rect, CvPoint *center_point);

// 获取两点的距离
float get_distance(CvPoint *point1, CvPoint *point2);

// 放大框
// void enlarge_rect(if_rect_t &face_rect_orig, cv::Rect &roi_enlarge, int image_width, int image_height);

// 计算跟踪物体的速度
float cal_v_std(int r, int v);

void validate_rect(cv::Rect *prect, int width, int height);
void rectangle2square(cv::Rect *prect, float exth_scale);
void square2rectangle(cv::Rect *prect, float exth_scale);
void enlarge_rect(cv::Rect *prect, float extw_scale, float exth_scale);
cv::Mat CropImageStd(const cv::Mat &img, int x1, int y1, int x2, int y2, float exth_scale_, float extw_scale_);
cv::Rect flip_rect(const cv::Rect &rect, int img_width);
std::vector<int> rect2point_posi(const cv::Rect &rect);
cv::Rect point_posi2rect(const std::vector<int> &point_posi);
std::vector<int> flip_point_posi(const std::vector<int> &point_posi, int img_width);
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_UTILSFUNCTION_H_
