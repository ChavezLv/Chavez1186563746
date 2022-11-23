//
// Created by Admin on 2021/6/24.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACIALLANDMARK_UTILS_FACELANDMARKUTILS_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACIALLANDMARK_UTILS_FACELANDMARKUTILS_H_
#include "include/arctern.h"
#include "src/common/algUtil.h"

#if __arm__
#include "common/neonmathfun.h"
#endif


namespace arctern{
template<typename T>
T getFace68(int imgW, int imgH, const T &box)  {
  int semi_expand_edge = static_cast<int>(abs(box.width - box.height) / 2);
  int edge_max;
  T box2;
  box2.x = box.x;
  box2.y = box.y;
  box2.width = box.width;
  box2.height = box.height;
  if (box2.width > box2.height) {
    edge_max = box2.width;
    box2.y -= semi_expand_edge;
    box2.height = box2.width;
  } else {
    edge_max = box2.height;
    box2.x -= semi_expand_edge;
    box2.width = box2.height;
  }

  semi_expand_edge = static_cast<int>(edge_max / 20);

  int xmin = std::max(0, static_cast<int>(box2.x - semi_expand_edge));
  int ymin = std::max(0, static_cast<int>(box2.y - semi_expand_edge));
  int xmax = std::min(static_cast<int>(imgW - 1),
                      static_cast<int>(box2.x + box2.width + semi_expand_edge));
  int ymax = std::min(static_cast<int>(imgH - 1),
                      static_cast<int>(box2.y + box2.height + semi_expand_edge));
  int tmpW = xmax - xmin;
  int tmpH = ymax - ymin;
  int min_edge = tmpW > tmpH ? tmpH : tmpW;
  box2.x = xmin;
  box2.y = ymin;
  box2.width = min_edge;
  box2.height = min_edge;
  return box2;
}

template<typename T>
T getFaceTiny(int imgW, int imgH, const T &box)  {
  const float scale = 1.125f;
  int w = box.width;
  int h = box.height;
  float x1 = box.x;
  float y1 = box.y;

  int scaled_w = static_cast<int>(w * scale + 0.5);
  int scaled_h = static_cast<int>(h * scale + 0.5);
  x1 = x1 - (scaled_w - w) / 2;
  y1 = y1 - (scaled_h - h) / 2;

  x1 = std::min(imgW - 1, std::max(0, static_cast<int>(x1 + 0.5)));
  y1 = std::min(imgH - 1, std::max(0, static_cast<int>(y1 + 0.5)));
  float x2 = std::min(imgW - 1, std::max(0, static_cast<int>(x1 + 0.5) + scaled_w));
  float y2 = std::min(imgH - 1, std::max(0, static_cast<int>(y1 + 0.5) + scaled_h));

  T ret;
  ret.x = x1;
  ret.y = y1;
  ret.width = x2-x1;
  ret.height = y2-y1;
  return ret;
}


template<typename T>
inline ArcternPoint geneOriLandmark(float tmpX, float tmpY, const T faceRect) {

#if __arm__
  float32x4_t a = {tmpY,-tmpY, tmpX, -tmpX};
  float32x4_t b = exp_ps(a);
  float expY1 = b[0];
  float expY2 = b[1];
  float expX1 = b[2];
  float expX2 = b[3];
#else
  float expY1 = std::exp(tmpY);
  float expY2 = std::exp(-tmpY);
  float expX1 = std::exp(tmpX);
  float expX2 = std::exp(-tmpX);
#endif

  int coor_y = (expY1 - expY2) / (expY1 + expY2) * faceRect.height + faceRect.y;

  int coor_x = (expX1 - expX2) / (expX1 + expX2) * faceRect.width + faceRect.x;
  arctern::ArcternPoint point;
  point.x = coor_x;
  point.y = coor_y;
  return point;
}

inline int GetLandmarkNum(const std::string &modelName) {
  auto pureModelName = getPureModelName(modelName);
  int ret = 68;
  ///  model name may not best way. but no better way now.
  if (pureModelName.find("tiny") != pureModelName.npos) {
    ret = 7;
  }
  return ret;
}  // namespace faceLandmark
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACIALLANDMARK_UTILS_FACELANDMARKUTILS_H_
