/*
* @file utilsfunction.cpp
* @date 2016-03-08
* @brief some utilis function in tracking.
* @author ZhangZhaofeng(zhang.zhaofeng@intellif.com)
*
* Copyright (c) 2016, Shenzhen Intellifusion Technologies Co., Ltd.
*/

#include "src/Algorithms/faceTracker/object_tracker/utilsfunction.h"
#include <algorithm>
#include <vector>

using cv::Rect;

float RectangleIntersect(cv::Rect *rect1, cv::Rect *rect2) {
  if (rect1 == NULL || rect2 == NULL) {
    return 0;
  }
  int minX, minY, maxX, maxY, nArea;
  int nArea1, nArea2, nAreaMin;

  nArea1 = rect1->width * rect1->height;
  nArea2 = rect2->width * rect2->height;
  nAreaMin = nArea1 < nArea2 ? nArea1 : nArea2;

  minX = MAX(rect1->x, rect2->x);
  minY = MAX(rect1->y, rect2->y);
  maxX = MIN(rect1->br().x, rect2->br().x);
  maxY = MIN(rect1->br().y, rect2->br().y);

  if (!(minX > maxX || minY > maxY)) {
    nArea = (maxX - minX) * (maxY - minY);
    return nArea / static_cast<float>(nAreaMin);
  }

  return 0;
}

bool BoxPosFilter(cv::Rect *rect, int nWidth, int nHeight, int nMargin) {
  if (rect == NULL) {
    return false;
  }

  if (rect->br().y + nMargin > nHeight) {
    return true;
  }

  if (rect->br().x + nMargin > nWidth) {
    return true;
  }

  if (rect->tl().x < nMargin) {
    return true;
  }

  if (rect->tl().y < nMargin) {
    return true;
  }

  return false;
}

bool BoxSizeFilter(cv::Rect *rect, int nDim) {
  if (rect == NULL) {
    return false;
  }

  if (rect->width > nDim) {
    return true;
  }

  if (rect->height > nDim) {
    return true;
  }

  return false;
}

bool Rule1(int R, int G, int B) {
  int diff = 15;  // orig: diff = 15;
  // bool e1 = (R>95) && (G>40) && (B>20) &&
  // ((max(R,max(G,B)) - min(R, min(G,B)))>diff) && (abs(R-G)>diff) && (R>G) && (R>B);
  bool e1 = (R > 95) && (G > 40) && (B > 20) && ((std::max(R, std::max(G, B)) - std::min(R, std::min(G, B))) > diff)
      && (abs(R - G) > diff) && (R > G) && (R > B);
  // bool e2 = (R>220) && (G>210) && (B>170) && (abs(R-G)<=15) && (R>B) && (G>B);
  bool e2 = (R > 220) && (G > 210) && (B > 170) && (abs(R - G) <= 15) && (R > B) && (G > B);
  return (e1 || e2);
}

int getSkin(cv::Mat const &src) {
  int cnt = 0;
  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.cols; j++) {
      cv::Vec3b pix_bgr = src.ptr<cv::Vec3b>(i)[j];
      int B = pix_bgr.val[0];
      int G = pix_bgr.val[1];
      int R = pix_bgr.val[2];
      bool a = Rule1(R, G, B);
      if (a)
        cnt++;
    }
  }
  return cnt;
}

// 人脸框转opencv rect
// void ifrt2Rect(cv::Rect *tracker_rect, if_rect_t *detect_rect, int image_width, int image_height)
// {
// int iamge_left = (detect_rect->left < 0) ? 0 : detect_rect->left;
// int image_right = (detect_rect->right > (image_width-1)) ? (image_width-1) : detect_rect->right;
// int image_top = (detect_rect->top < 0) ? 0 : detect_rect->top;
// int image_bottom = (detect_rect->bottom > (image_height - 1)) ? (image_height - 1) : detect_rect->bottom;
//
// tracker_rect->x = iamge_left;
// tracker_rect->y = image_top;
// tracker_rect->width = image_right - iamge_left + 1;
// tracker_rect->height = image_bottom - image_top + 1;
// }

// 获取框的中心
void get_rect_center(cv::Rect *tracker_rect, CvPoint *center_point) {
  center_point->x = tracker_rect->x + tracker_rect->width / 2;
  center_point->y = tracker_rect->y + tracker_rect->height / 2;
}

// 获取两点的距离
float get_distance(CvPoint *point1, CvPoint *point2) {
  float p_dis = sqrt(pow((point1->x - point2->x), 2) + pow((point1->y - point2->y), 2));
  return p_dis;
}

// 放大框
// void enlarge_rect(if_rect_t &face_rect_orig, cv::Rect &roi_enlarge, int image_width, int image_height)
// {
// //将图像每边扩大原来的1/4
// int width_enlarge = (face_rect_orig.right - face_rect_orig.left) / 4;
// int height_enlarge = (face_rect_orig.bottom - face_rect_orig.top) / 4;
//
// int rect_left_enlarge = (face_rect_orig.left > width_enlarge)
//  ? (face_rect_orig.left - width_enlarge)
// : 0;
// int rect_right_enlarge = (face_rect_orig.right < (image_width - width_enlarge))
// ? (face_rect_orig.right + width_enlarge)
// : image_width;
//
// int rect_top_enlarge = (face_rect_orig.top  > height_enlarge)
//  ? (face_rect_orig.top - height_enlarge)
//  : 0;
// int rect_bottom_enlarge = (face_rect_orig.bottom < (image_height - height_enlarge))
//  ? (face_rect_orig.bottom + height_enlarge)
//  : image_height;
//
// roi_enlarge = cvRect(rect_left_enlarge,
//  rect_top_enlarge,
//  rect_right_enlarge - rect_left_enlarge,
//  rect_bottom_enlarge - rect_top_enlarge);
// }

// 计算跟踪物体的速度
float cal_v_std(int r, int v) {
  float r_std = 640;
  float b0 = 0.05;
  float v_ratio = pow((r_std / r), 0.5) + b0;
  float v_std = v_ratio * v;
  return v_std;
}

void validate_rect(cv::Rect *prect, int width, int height) {
  if (prect->x < 0) {
    prect->width = prect->width - abs(prect->x);
    prect->x = 0;
  }

  if (prect->y < 0) {
    prect->height = prect->height - abs(prect->y);
    prect->y = 0;
  }

  if (prect->x > width - 1) {
    prect->x = width - 1;
    prect->width = 0;
  }

  if (prect->y > height - 1) {
    prect->y = height - 1;
    prect->height = 0;
  }

  if (prect->width < 0) {
    prect->width = 0;
  }

  if (prect->width > width - prect->x) {
    prect->width = width - prect->x;
  }

  if (prect->height < 0) {
    prect->height = 0;
  }

  if (prect->height > height - prect->y) {
    prect->height = height - prect->y;
  }
}

void rectangle2square(cv::Rect *prect, float exth_scale) {
  prect->y += prect->height * (1.0 - 1.0 / (1 + 2.0 * exth_scale)) * 0.5;
  prect->height = prect->height / (1 + 2.0 * exth_scale);
}
void square2rectangle(cv::Rect *prect, float exth_scale) {
  prect->y -= prect->height * exth_scale;
  prect->height = (prect->height * (1 + 2 * exth_scale));
}

void enlarge_rect(cv::Rect *prect, float extw_scale, float exth_scale) {
  prect->x = static_cast<int>(prect->x - prect->width * extw_scale);
  prect->width = static_cast<int>(prect->width + prect->width * 2 * extw_scale);
  prect->y = static_cast<int>(prect->y - prect->height * exth_scale);
  prect->height = static_cast<int>(prect->height + prect->height * 2 * exth_scale);
}

cv::Mat CropImageStd(const cv::Mat &img, int x1, int y1, int x2, int y2, float exth_scale_, float extw_scale_) {
  int img_h = img.rows;
  int img_w = img.cols;
  int w = x2 - x1 + 1;
  int h = y2 - y1 + 1;
  // CHECK(w == h) << "input face box must be square";
  // CHECK(x2 > x1 && y2 > y1) << "input face box is illegal";
  // TODO(make sure mtcnn detection result is illegal)
  x1 -= 0.5 * (w * (1 + 2.0 * extw_scale_) - w);
  y1 -= 0.5 * (h * (1 + 2.0 * exth_scale_) - h);
  x2 = x1 + w * (1 + 2 * extw_scale_) - 1;
  // y2 = y1 + h*(1 + 2 * exth_scale_) - 0;
  y2 = y1 + h * (1 + 2 * exth_scale_) - 1;
  x1 = std::min(img_w, std::max(0, x1));
  y1 = std::min(img_h, std::max(0, y1));
  x2 = std::min(img_w, std::max(0, x2));
  y2 = std::min(img_h, std::max(0, y2));
  // w = x2 - x1 + 1;
  // h = y2 - y1 + 1;
  // cv::Mat roi = cv::Mat::zeros(h, w, img.type());
  cv::Mat roi;
  int dwRectWidth = (x2 + 1) > img.cols ? (img.cols - x1) : (x2 - x1 + 1);
  int dwRectHeight = (y2 + 1) > img.rows ? (img.rows - y1) : (y2 - y1 + 1);
  // cv::Rect rect(x1, y1, x2-x1+1, y2-y1+1);
  cv::Rect rect(x1, y1, dwRectWidth, dwRectHeight);
  img(rect).copyTo(roi);
  return roi;
}

cv::Rect flip_rect(const cv::Rect &rect, int img_width) {
  cv::Rect rect_flip = rect;
  int rect_left = ((img_width - 1) - (rect.x + rect.width - 1)) > 0 ? ((img_width - 1) - (rect.x + rect.width - 1)) : 0;
  int rect_right = ((img_width - 1) - rect.x) > 0 ? ((img_width - 1) - rect.x) : 0;
  rect_flip.x = rect_left;
  rect_flip.width = rect_right - rect_left + 1;
  return rect_flip;
}

// transform rect to point position(left,top,right,bottom)
std::vector<int> rect2point_posi(const cv::Rect &rect) {
  std::vector<int> point_posi;
  int left = rect.x;
  int top = rect.y;
  int right = rect.x + rect.width - 1;
  int bottom = rect.y + rect.height - 1;
  point_posi.push_back(left);
  point_posi.push_back(top);
  point_posi.push_back(right);
  point_posi.push_back(bottom);
  return point_posi;
}

// transform point position(left,top,right,bottom) to rect
cv::Rect point_posi2rect(const std::vector<int> &point_posi) {
  int x = point_posi[0];
  int y = point_posi[1];
  int width = point_posi[2] - point_posi[0] + 1;
  int height = point_posi[3] - point_posi[1] + 1;
  cv::Rect rect = cvRect(x, y, width, height);
  return rect;
}

// flip point_posi
std::vector<int> flip_point_posi(const std::vector<int> &point_posi, int img_width) {
  std::vector<int> point_posi_flip;
  point_posi_flip = point_posi;
  point_posi_flip[0] = (img_width - 1 - point_posi[2]) > 0 ? (img_width - 1 - point_posi[2]) : 0;
  point_posi_flip[2] = (img_width - 1 - point_posi[0]) > 0 ? (img_width - 1 - point_posi[0]) : 0;
  return point_posi_flip;
}
