//
// Created by Admin on 2021/7/2.
//
#include "personCompletenessUtil.h"
#include <assert.h>
#include <cmath>
namespace arctern{
void parse_truncation(const float * out, int vsize,
                             std::vector<float> &truncation) {
  assert(2 == vsize);
  truncation.push_back(out[0]);
  truncation.push_back(out[1]);
}

 void parse_visible_parts(const float *out, int vsize,
                                std::vector<bool> &visible_parts,
                                std::vector<float> &visible_parts_score) {
  assert(5 == vsize);
  for (int i = 0; i < vsize; ++i) {
    float z = out[i];
    float score = 1.0f / (1.0f + std::exp(-z));
    bool label = score > 0.5;
    visible_parts.push_back(label);
    visible_parts_score.push_back(score);
  }
}

 void parse_soft_mask( const float * out, int vsize,
                             std::vector<std::vector<bool>> &soft_mask,
                             std::vector<std::vector<float>> &soft_mask_score,
                             int rows, int cols , float thresh) {
  assert(rows * cols == vsize);
  int counter = 0;
  for (int r = 0; r < rows; ++r) {
    std::vector<bool> line;
    std::vector<float> line_score;
    for (int c = 0; c < cols; ++c) {
      float value = out[counter];
      bool label = value > thresh;
      line.push_back(label);
      line_score.push_back(value);
      counter++;
    }
    soft_mask.push_back(line);
    soft_mask_score.push_back(line_score);
  }
}

}