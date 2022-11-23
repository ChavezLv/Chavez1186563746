/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.26
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_FEATUREUTIL_H_
#define SRC_COMMON_FEATUREUTIL_H_

#include <math.h>
#include <vector>
#include "include/arctern.h"

namespace arctern {
void ARCTERN_EXPORT_API encrypt_feature(const std::vector<float> &feat,std::vector<uint8> & encrypted);
std::vector<uint8> encrypt_feature(const std::vector<float> &feat);
std::vector<float> decrypt_feature(const std::vector<uint8> &encrypted);

//float compute_similarity(const std::vector<uint8> &encrypted1,
//                         const std::vector<uint8> &encrypted2);

template <typename T>
void l2norm(std::vector<T> &vec) {
  float norm = 0.0;
  for (const auto &v : vec) {
    norm += v * v;
  }

  norm = sqrt(norm);

  for (auto &v : vec) {
    v /= norm;
  }
}

// This function is for feature norm
inline void l2norm(float* ptr, size_t n) {
  float norm = 0.0;
  for (size_t i = 0; i < n; ++i) {
    norm += ptr[i] * ptr[i];
  }

  norm = sqrt(norm);

  for (size_t i = 0; i < n; ++i) {
    ptr[i] /= norm;
  }
}

}  // namespace arctern
#endif  // SRC_COMMON_FEATUREUTIL_H_
