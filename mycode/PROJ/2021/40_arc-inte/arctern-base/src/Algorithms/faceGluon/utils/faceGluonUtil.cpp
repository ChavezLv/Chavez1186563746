//
// Created by chengaohua on 2021/6/17.
//

#include <map>
#include <iostream>
#include "faceGluonUtil.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"

namespace arctern {
namespace gluon {

LocalPreProcessV getProcessV(std::string &version) {
  const std::map<std::string, LocalPreProcessV> map = {
      {"1.9.0", LocalPreProcessV::V2},
      {"2.3.0", LocalPreProcessV::V1},
      {"2.4.0", LocalPreProcessV::V1},
      {"2.5.0", LocalPreProcessV::V1},
      {"3.0.0", LocalPreProcessV::V1},
      {"3.3.5", LocalPreProcessV::V1},
      {"3.4.0", LocalPreProcessV::V3},
      {"3.5.5", LocalPreProcessV::V1},
      {"3.6.0", LocalPreProcessV::V3},
      {"3.6.1", LocalPreProcessV::V3},
      {"3.7.0", LocalPreProcessV::V3},
      {"3.8.0", LocalPreProcessV::V3},
      {"3.9.0", LocalPreProcessV::V3},
      {"4.5.0", LocalPreProcessV::V1},
      {"3.8.6", LocalPreProcessV::V3},
      {"3.9.4", LocalPreProcessV::V4},
      {"5.1.0", LocalPreProcessV::V4},
      {"7.1.0", LocalPreProcessV::V5},
      {"7.2.0", LocalPreProcessV::V6},
      {"7.3.0", LocalPreProcessV::V6},
      {"7.4.0", LocalPreProcessV::V6},
      {"7.4.1", LocalPreProcessV::V6}
  };

  auto type = LocalPreProcessV::V3;
  if (map.find(version) == map.end()) {
    std::cout << " the model is not registered. now use landmark version " << std::endl;
  }else {
    type = map.at(version);
  }

  return type;
}

cv::Rect getFace(const int imgW, const int imgH, const cv::Rect &bbox,
                 const std::array<float, 2> exth_scale, const std::array<float, 2> extw_scale) {
  int x = bbox.x;
  int y = bbox.y;
  int w = bbox.width;
  int h = bbox.height;
  int x1 = roundf(fmin(imgW, fmax(0, x - w * extw_scale[0])));
  int y1 = roundf(fmin(imgH, fmax(0, y - h * exth_scale[0])));
  int x2 = roundf(fmin(imgW, fmax(0, x1 + w * (1 + 2 * extw_scale[1]))));
  int y2 = roundf(fmin(imgH, fmax(0, y1 + h * (1 + 2 * exth_scale[1]))));

  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}

cv::Rect getFaceV5(const int imgW, const int imgH, const cv::Rect& bbox,
                                     float exth_scale, float extw_scale) {

  int margin = 16;
  int model_h = 128;
  int model_w = 112;

  int x = bbox.tl().x;
  int y = bbox.tl().y;
  int w = bbox.width;
  int h = bbox.height;
  int x1 = roundf(fmin(imgW, fmax(0, x - w * extw_scale)));
  int y1 = roundf(fmin(imgH, fmax(0, y - h * exth_scale)));
  int x2 = roundf(fmin(imgW, fmax(0, x1 + w * (1 + 2 * extw_scale))));
  int y2 = roundf(fmin(imgH, fmax(0, y1 + h * (1 + 2 * exth_scale))));

  int scale_w = x2 - x1;
  int scale_h = y2 - y1;
  float m_x = 1.0 * margin * scale_w / model_w; /// margin
  float m_y = 1.0 * margin * scale_h / model_h; /// margin
  //std::cout << "m_x, m_y: " << m_x << " " << m_y << std::endl;

  x1 = int(roundf(fmin(imgW, fmax(0, x1 - m_x))));
  y1 = int(roundf(fmin(imgH, fmax(0, y1 - m_y))));
  x2 = int(roundf(fmin(imgW, fmax(0, x2 + m_x))));
  y2 = int(roundf(fmin(imgH, fmax(0, y2 + m_y))));

  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}

cv::Rect convert_to_square(const cv::Rect& bbox,float exth_scale,float extw_scale) {
  int max_side = 0, w = 0, h = 0;
  w = bbox.width;
  h = bbox.height;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  max_side = std::max(w, h);
  x1 -= 0.5 * (max_side * (1 + 2.0 * extw_scale) - w);  // TODO(float to int)
  y1 -= 0.5 * (max_side * (1 + 2.0 * exth_scale) - h);
  x2 = x1 + max_side * (1 + 2 * extw_scale) - 1;
  y2 = y1 + max_side * (1 + 2 * exth_scale) - 1;
  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}


cv::Rect getFaceV6(const int imgW, const int imgH, const cv::Rect& bbox,
                   float exth_scale, float extw_scale) {

  int margin = 16;
  int model_h = 128;
  int model_w = 112;

  int x1 = 0;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;

  int imgw = imgW;
  int imgh = imgH;

  cv::Rect square_bbox = bbox;
  if (bbox.width != bbox.height){
    square_bbox = gluon::convert_to_square(bbox, 0, 0);
    std::cout << "warning !!! The input bbox is not a square ! " << std::endl;
  }
  int x = square_bbox.tl().x;
  int y = square_bbox.tl().y;
  int w = square_bbox.width;
  int h = square_bbox.height;

  extw_scale = 0.06f;
  exth_scale = 0.14f;

  x1 = int(roundf(x - w * extw_scale));
  y1 = int(roundf(y - h * exth_scale));
  x2 = int(roundf(x1 + w * (1 + 2 * extw_scale)));
  y2 = int(roundf(y1 + h * (1 + 2 * exth_scale)));

  int scale_w = x2 - x1;
  int scale_h = y2 - y1;
  int m_x = int(roundf(1.0 * margin * scale_w / model_w)); /// margin
  int m_y = int(roundf(1.0 * margin * scale_h / model_h)); /// margin

  x1 = x1 - m_x;
  y1 = y1 - m_y;
  x2 = x2 + m_x;
  y2 = y2 + m_y;

  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}

std::vector<ArcternPoint> calcAlignFacePoints(float w, float h, const std::vector<ArcternPoint> &lmks) {
  assert(lmks.size() == 68 || lmks.size() == 7);

  std::vector<ArcternPoint> afLmks;
  afLmks.resize(2);

  if (68 == lmks.size()) {
    afLmks[0].x = std::min(
        w, std::max(0.0f,
                    static_cast<float>((lmks[36].x + lmks[39].x) / 2.0)));
    afLmks[0].y = std::min(
        h, std::max(0.0f,
                    static_cast<float>((lmks[36].y + lmks[39].y) / 2.0)));
    afLmks[1].x = std::min(
        w, std::max(0.0f,
                    static_cast<float>((lmks[42].x + lmks[45].x) / 2.0)));
    afLmks[1].y = std::min(
        h, std::max(0.0f,
                    static_cast<float>((lmks[42].y + lmks[45].y) / 2.0)));

  } else {
    afLmks[0].x = std::min(
        w, std::max(0.0f,
                    static_cast<float>((lmks[1].x + lmks[2].x) / 2.0)));
    afLmks[0].y = std::min(
        h, std::max(0.0f,
                    static_cast<float>((lmks[1].y + lmks[2].y) / 2.0)));
    afLmks[1].x = std::min(
        w, std::max(0.0f,
                    static_cast<float>((lmks[3].x + lmks[4].x) / 2.0)));
    afLmks[1].y = std::min(
        h, std::max(0.0f,
                    static_cast<float>((lmks[3].y + lmks[4].y) / 2.0)));
  }

  return afLmks;
}

std::vector<uint8> encryptFeature(const std::vector<float> &feat) {
  return encrypt_feature(feat);
}

}  // end namespace gluon
}  // end namespace arctern

