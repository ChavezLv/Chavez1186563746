//
// Created by chengaohua on 2021/6/24.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACESMOKE_UTILS_FACESMOKEUTIL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACESMOKE_UTILS_FACESMOKEUTIL_H_

#include "opencv2/opencv.hpp"

namespace arctern {
namespace smoke {

cv::Rect getFace(int width, int height, const cv::Rect &bbox);

std::pair<int, float> calcSmoke(const float *data, size_t num_class);

}
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACESMOKE_UTILS_FACESMOKEUTIL_H_
