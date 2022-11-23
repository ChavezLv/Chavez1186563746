/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.19
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <algorithm>
#include "./algUtil.h"

namespace arctern {
cv::Rect check_shrink_box(const cv::Rect &box, int image_width,
                          int image_height) {
  int x1 = box.x;
  int y1 = box.y;
  int x2 = box.br().x;
  int y2 = box.br().y;

  x1 = std::max(0, std::min(x1, image_width));
  y1 = std::max(0, std::min(y1, image_height));
  x2 = std::max(x1 + 1, std::min(x2, image_width));
  y2 = std::max(y1 + 1, std::min(y2, image_height));

  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}

int convertFloat2Int(float ori, Float2Int type) {
  if (type == Float2Int::ROUNDING) {
    return static_cast<int>(ori + 0.5);
  } else {
    return static_cast<int>(ori);
  }
}

cv::Rect get_face(int width, int height, const cv::Rect &bbox, std::array<float, 4> extScale, Float2Int type, bool square_rect) {
  
  int w = bbox.width + 1;
  int h = bbox.height + 1;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  
  x1 = x1 - w * extScale[0];
  y1 = y1 - h * extScale[1];
  x2 = x2 + w * extScale[2];
  y2 = y2 + h * extScale[3];
  float by1 = std::min(std::max(0, convertFloat2Int(y1, type)), height - 1);
  float by2 = std::min(std::max(0, convertFloat2Int(y2, type)), height - 1);
  float hh = by2 - by1 + 1;
  float ww = hh;
  if (square_rect && ww > w) {
    x1 = x1 - (ww - w) / 2;
    x2 = x2 + (ww - w) / 2;
  }
  float bx1 = std::min(std::max(0, convertFloat2Int(x1, type)), width - 1);
  float bx2 = std::min(std::max(0, convertFloat2Int(x2, type)), width - 1);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}

std::string getPureModelName(const std::string &modelName) {
  std::string pureModelName = modelName;
  /// get pure model name
  auto findVirgulePos = modelName.find_last_of('/');
  if (findVirgulePos != modelName.npos) {
    pureModelName = modelName.substr(findVirgulePos + 1);  /// dropout '/'
  }
  return pureModelName;
}

int python_round(float input) {
  int low_end = static_cast<int>(input);
  float diff = input - low_end;
  if (fabs(diff - 0.5f) < 0.00000001) {
    return (low_end % 2 == 0) ? low_end : low_end + 1;
  } else {
    return static_cast<int>(input + 0.5);
  }
}

void split(const std::string& s, const std::string& c,
           std::vector<std::string>& ss) {
  std::string::size_type spos(0);
  std::string::size_type epos;

  while (std::string::npos != (epos = s.find(c, spos))) {
    ss.push_back(s.substr(spos, epos - spos));
    spos = epos + c.size();
  }

  if (spos != s.length()) {
    ss.push_back(s.substr(spos));
  }
}

}  // namespace arctern
