//
// Created by chengaohua on 2021/6/24.
//
#include "faceSmokeUtil.h"
#include "common/algUtil.h"

namespace arctern {
namespace smoke {
/// \brief offset 1 pixel may cause the large difference. so use special get_face
cv::Rect getFace(int width, int height, const cv::Rect &bbox) {
  std::array<float, 4> extScale{0.0, 0.5, 0.0, 0.1};
  int w = bbox.width + 1;
  int h = bbox.height + 1;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;

  y1 = y1 - h * extScale[1];
  y2 = y2 + h * extScale[3];
  float by1 = std::min(std::max(0, static_cast<int>(y1)), height - 1);
  float by2 = std::min(std::max(0, static_cast<int>(y2)), height - 1);
  float hh = by2 - by1 + 1;
  float ww = hh;
  if (ww > w) {
    int wx = (int) (ww - w) / 2;
    x1 = x1 - wx;
    x2 = x2 + wx;
  }
  float bx1 = std::min(std::max(0, static_cast<int>(x1)), width - 1);
  float bx2 = std::min(std::max(0, static_cast<int>(x2)), width - 1);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}

/// \brief calculate phone label and corresponding score
std::pair<int, float> calcSmoke(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

}
}