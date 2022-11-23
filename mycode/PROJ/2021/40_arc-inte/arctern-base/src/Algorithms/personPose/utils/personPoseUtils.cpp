//
// Created by chengaohua on 2021/7/7.
//

#include "personPoseUtils.h"

namespace arctern {
namespace personPose {
std::vector<std::pair<cv::Point, float>> heatmap_to_coord(float *arr, cv::Rect bbox) {
  const int m_iHeatMapH = 64 ;
  const int m_iHeatMapW = 48;
  const int m_iNumsKeyPoints = 17;
  std::vector<std::pair<cv::Point, float>> result(m_iNumsKeyPoints);
  int index_shift = m_iHeatMapH * m_iHeatMapW;

  for (int i = 0; i < m_iNumsKeyPoints; i++) {
    float max_val = arr[0];
    int max_index = 0;
    for (int j = 0; j < m_iHeatMapH * m_iHeatMapW; j++) {
      float val = arr[j];
      if (val > max_val) {
        max_val = val;
        max_index = j;
      }
    }
    int max_row_index = int(max_index / m_iHeatMapW);
    int max_col_index = max_index % m_iHeatMapW;
    float w_ratio = max_col_index / float(m_iHeatMapW);
    float h_ratio = max_row_index / float(m_iHeatMapH);
    max_row_index = bbox.width * w_ratio + bbox.x;
    max_col_index = bbox.height * h_ratio + bbox.y;
    result[i] =
        std::make_pair(cv::Point(max_row_index, max_col_index), max_val);
    arr += index_shift;
  }

  return result;
}

cv::Rect upscaleBbox(cv::Rect bbox, float scale) {
  int center_x = bbox.x + bbox.width / 2;
  int center_y = bbox.y + bbox.height / 2;
  int ext_x = center_x - bbox.width / 2 * scale;
  int ext_y = center_y - bbox.height / 2 * scale;
  int ext_width = bbox.width * scale;
  int ext_height = bbox.height * scale;

  return cv::Rect(ext_x, ext_y, ext_width, ext_height);
}

}
}