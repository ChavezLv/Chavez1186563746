/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceTracker/object_tracker/overlap.h"

seRectPair::seRect::seRect(cv::Rect r) {
  startX = r.x;
  startY = r.y;
  endX = r.x + r.width - 1;
  endY = r.y + r.height - 1;
}

seRectPair::seRectPair(cv::Rect cvr1, cv::Rect cvr2) {
  this->r1 = seRect(cvr1);
  this->r2 = seRect(cvr2);
  this->r_intersect.startX = r1.startX > r2.startX ? r1.startX : r2.startX;
  this->r_intersect.endX = r1.endX < r2.endX ? r1.endX : r2.endX;
  this->r_intersect.startY = r1.startY > r2.startY ? r1.startY : r2.startY;
  this->r_intersect.endY = r1.endY < r2.endY ? r1.endY : r2.endY;
  union_area = static_cast<float>(cvr1.width * cvr1.height + cvr2.width * cvr2.height);
}

float seRectPair::PairIntersect() {
  float intersect_area = 0;
  if ((r_intersect.startX <= r_intersect.endX) && (r_intersect.startY <= r_intersect.endY)) {
    intersect_area = (r_intersect.endX - r_intersect.startX + 1) * (r_intersect.endY - r_intersect.startY + 1);
  } else {
    intersect_area = 0;
  }
  return intersect_area;
}

float seRectPair::PairUnion() {
  return union_area;
}

float seRectPair::getOverlapRatio() {
  float v_ret = 0;
  float v_inters = PairIntersect();
  float v_union = PairUnion();

  if (v_union - v_inters == 0) {
    v_ret = 0;
  } else {
    v_ret = v_inters / (v_union - v_inters);
    // std::cout << "v_inters = " << v_inters << std::endl;
    // std::cout << "v_union = " << v_union << std::endl;
    // std::cout << "v_ret = " << v_ret << std::endl;
  }
  return v_ret;
}

float seRectPair::getSelfOverlapRatio() {
  float v_ret = 0;
  float v_inters = PairIntersect();
  float v_r1 = (r1.endX - r1.startX + 1) * (r1.endY - r1.startY + 1);

  if (v_r1 == 0) {
    v_ret = 0;
  } else {
    v_ret = v_inters / (v_r1);
    // std::cout << "v_inters = " << v_inters << std::endl;
    // std::cout << "v_union = " << v_union << std::endl;
    // std::cout << "v_ret = " << v_ret << std::endl;
  }
  return v_ret;
}

float seRectPair::getMaxOverlapRatio() {
  float v_ret = 0;
  float v_inters = PairIntersect();
  float v_r1 = (r1.endX - r1.startX + 1) * (r1.endY - r1.startY + 1);
  float v_r2 = (r2.endX - r2.startX + 1) * (r2.endY - r2.startY + 1);
  float v_min = v_r1 < v_r2 ? v_r1 : v_r2;
  if (v_min == 0) {
    v_ret = 0;
  } else {
    v_ret = v_inters / (v_min);
  }
  return v_ret;
}
