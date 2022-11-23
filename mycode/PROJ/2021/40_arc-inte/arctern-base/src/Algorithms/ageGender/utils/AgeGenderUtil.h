//
// Created by Admin on 2021/7/26.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_UTILS_AGEGENDERUTIL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_UTILS_AGEGENDERUTIL_H_
#include <utility>
#include <cstddef>
std::pair<int, float> calc_gender(const float *data, size_t num_class);
std::pair<int, float> calc_age(const float *data, size_t num_class);
#endif //ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_UTILS_AGEGENDERUTIL_H_
