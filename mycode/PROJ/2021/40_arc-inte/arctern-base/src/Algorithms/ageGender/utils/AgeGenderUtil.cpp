//
// Created by Admin on 2021/7/26.
//

#include "AgeGenderUtil.h"
#include <vector>
#include "src/common/algUtil.h"
/// \brief calculate gender label and corresponding score
std::pair<int, float> calc_gender(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

///> calculate age and get maximum score
std::pair<int, float> calc_age(const float *data, size_t num_class) {
  float age = 0;
  float score = 0;
  for (size_t j = 0; j < num_class; ++j) {
    age += data[j] * j;
    if (score < data[j]) {
      score = data[j];
    }
  }
  return std::make_pair(static_cast<int>(age + 0.5), score);
}