//
// Created by chengaohua on 2021/6/17.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_FACEGLUONUTIL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_FACEGLUONUTIL_H_

#include "opencv2/opencv.hpp"
#include "include/arctern.h"

namespace arctern {
namespace gluon {

enum class LocalPreProcessV {
  V1, V2, V3, V4, V5, V6
};

LocalPreProcessV getProcessV(std::string &version);


cv::Rect getFace(const int imgW, const int imgH, const cv::Rect &bbox,
                 const std::array<float, 2> exth_scale, const std::array<float, 2> extw_scale);

cv::Rect getFaceV5(const int imgW, const int imgH, const cv::Rect& bbox,
                   float exth_scale, float extw_scale);

cv::Rect getFaceV6(const int imgW, const int imgH, const cv::Rect& bbox,
                   float exth_scale, float extw_scale);

std::vector<ArcternPoint> calcAlignFacePoints(float w, float h, const std::vector<ArcternPoint> &lmks);

std::vector<uint8> encryptFeature(const std::vector<float> &feat);
}
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_FACEGLUONUTIL_H_
