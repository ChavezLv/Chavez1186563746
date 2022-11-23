//
// Created by chengaohua on 2021/7/7.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONPOSE_UTILS_PERSONPOSEUTILS_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONPOSE_UTILS_PERSONPOSEUTILS_H_

#include "opencv2/opencv.hpp"

namespace arctern {
namespace personPose {
std::vector<std::pair<cv::Point, float>> heatmap_to_coord(float *arr, cv::Rect bbox);
cv::Rect upscaleBbox(cv::Rect bbox, float scale);
}
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONPOSE_UTILS_PERSONPOSEUTILS_H_
