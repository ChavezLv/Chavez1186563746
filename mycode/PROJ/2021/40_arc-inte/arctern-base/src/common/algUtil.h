/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.19
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_ALGUTIL_H_
#define SRC_COMMON_ALGUTIL_H_
#include <algorithm>
#include <vector>
#include "opencv2/opencv.hpp"

namespace arctern {

/// \brief adjust rect , prevent over edge
/// \param box - ori rect
/// \param image_height - image height
/// \param image_width - image width
/// \return adjusted rect
cv::Rect check_shrink_box(const cv::Rect &box, int image_width,
                          int image_height);

enum class Float2Int{
  ROUNDING = 0 ,
  TRUNCATION = 1,
};

cv::Rect get_face(int width, int height, const cv::Rect &bbox, std::array<float, 4> extScale, Float2Int type = Float2Int::ROUNDING, bool square_rect = true);

template<typename T>
std::vector<T> convert_to_probability(const std::vector<T> &input) {
  std::vector<float> prob(input);
  auto maxval_it = std::max_element(prob.begin(), prob.end());
  float maxval = *maxval_it;
  std::for_each(prob.begin(), prob.end(),
                [maxval](float &v) { v = std::exp(v - maxval); });
  float sum = 0.0f;
  std::for_each(prob.begin(), prob.end(), [&sum](float v) { sum += v; });
  std::for_each(prob.begin(), prob.end(), [sum](float &v) { v /= sum; });

  return prob;

}

template<typename T>
int softmax(const std::vector<T> &input, float &prob) {
  auto probs = convert_to_probability(input);

  auto max_iter = std::max_element(probs.begin(), probs.end());
  int maxidx = max_iter - probs.begin();
  prob = *max_iter;
  return maxidx;

}

template <typename T>
int get_label(const std::vector<T> &input, float &prob, float thresh) {
  auto probs = convert_to_probability(input);
  int label = 1;
  if (probs[1] < thresh) {
    label = 0;
  }
  prob = probs[1]; // get the confidence of positive
  return label;
}


std::string getPureModelName(const std::string &modelName);

int python_round(float input);

void split(const std::string &s, const std::string &c,
           std::vector<std::string> &ss);

}// namespace arctern

#endif  // SRC_COMMON_ALGUTIL_H_
