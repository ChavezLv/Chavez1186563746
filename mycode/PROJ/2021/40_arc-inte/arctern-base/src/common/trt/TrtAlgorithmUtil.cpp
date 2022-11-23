//
// Created by Admin on 2021/6/18.
//

#include "TrtAlgorithmUtil.h"
#include "Base/base.h"
ArcternRect CheckShrinkBox(const ArcternRect& box, int image_width,int image_height) {
  int x1 = (int) box.x;
  int y1 = (int) box.y;
  int x2 = (int) (x1 + box.width);
  int y2 = (int) (y1 + box.height);

  x1 = std::max(0, std::min(x1, image_width));
  y1 = std::max(0, std::min(y1, image_height));
  x2 = std::max(0, std::min(x2, image_width));
  y2 = std::max(0, std::min(y2, image_height));

  ArcternRect ret;
  ret.x = x1;
  ret.y = y1;
  ret.width = x2 - x1;
  ret.height = y2 - y1;
  return ret;
}

int convertFloat2Int(float ori, Float2Int type) {
  if (type == Float2Int::ROUNDING) {
    return static_cast<int>(ori + 0.5);
  } else {
    return static_cast<int>(ori);
  }
}

ArcternRect GetFace(int width, int height, const ArcternRect&bbox, std::array<float, 4> extScale, Float2Int type, bool square_rect) {
  int w = bbox.width + 1;
  int h = bbox.height + 1;
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = x1 + bbox.width;
  float y2 = y1 + bbox.height;

  x1 = x1 - w * extScale[0];
  y1 = y1 - h * extScale[1];
  x2 = x2 + w * extScale[2];
  y2 = y2 + h * extScale[3];
  float by1 = std::min(std::max(0, convertFloat2Int(y1, type)), height - 1);
  float by2 = std::min(std::max(0, convertFloat2Int(y2, type)), height - 1);
  float hh = by2 - by1 + 1;
  float ww = hh;
  if (square_rect && ww > w) {
    x1 = x1 - (ww - w) / 2;
    x2 = x2 + (ww - w) / 2;
  }
  float bx1 = std::min(std::max(0, convertFloat2Int(x1, type)), width - 1);
  float bx2 = std::min(std::max(0, convertFloat2Int(x2, type)), width - 1);

  ArcternRect ret;
  ret.x = bx1;
  ret.y = by1;
  ret.width = bx2 - bx1;
  ret.height = by2 - by1;
  return ret;
}

