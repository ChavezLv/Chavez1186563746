/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.17
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <vector>
#include <algorithm>
#include "detectUtil.h"

using arctern::ArcternRect;

float overlap(float x1, float w1, float x2, float w2) {
  float l1 = x1 - w1 / 2;
  float l2 = x2 - w2 / 2;
  float left = l1 > l2 ? l1 : l2;
  float r1 = x1 + w1 / 2;
  float r2 = x2 + w2 / 2;
  float right = r1 < r2 ? r1 : r2;
  return right - left;
}

float box_intersection(ArcternRect a, ArcternRect b) {
  float w = overlap(a.x, a.width, b.x, b.width);
  float h = overlap(a.y, a.height, b.y, b.height);
  if (w < 0 || h < 0) return 0;
  float area = w * h;
  return area;
}

float box_union(ArcternRect a, ArcternRect b) {
  float i = box_intersection(a, b);
  float u = a.width * a.height + b.width * b.height - i;
  return u;
}

namespace arctern {

float box_iou(ArcternRect a, ArcternRect b) {
  return box_intersection(a, b) / box_union(a, b);
}

void ConvertSquare(Vbbox &bbox, float exth_scale, float extw_scale) {
  int max_side = 0, w = 0, h = 0;
  for (size_t i = 0; i < bbox.size(); i++) {
    w = bbox[i].x2 - bbox[i].x1 + 1;
    h = bbox[i].y2 - bbox[i].y1 + 1;
    max_side = std::max(w, h);
    bbox[i].x1 -=
        (0.5 * (max_side * (1 + 2.0 * extw_scale) - w));  // TODO(float to int)
    bbox[i].y1 -= (0.5 * (max_side * (1 + 2.0 * exth_scale) - h));
    bbox[i].x2 = bbox[i].x1 + max_side * (1 + 2 * extw_scale) - 1;
    bbox[i].y2 = bbox[i].y1 + max_side * (1 + 2 * exth_scale) - 1;
  }
}
}  // namespace arctern



