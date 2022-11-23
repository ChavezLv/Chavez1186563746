//
// Created by chengaohua on 2021/6/24.
//

#include "personHeadwearUtil.h"
#include <vector>
#include "common/algUtil.h"
namespace arctern {
namespace personHeadwear {
std::pair<int, float> calcHeadwear(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  max_score = *max_element(prob.begin(), prob.end());
  return std::make_pair(max_idx, max_score);
}

}
}