/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <vector>
#include "src/Algorithms/faceTracker/object_tracker/GluonFeature.h"
// using namespace std;
// using namespace cv;
using std::vector;
using cv::Rect;
using cv::Mat;
using std::cout;
using std::endl;

vector<float> CalcFeature(Mat &frame_color, Rect rect) {
//
//    int x1 = rect.x < 0 ? 0 : rect.x;
//    int y1 = rect.y < 0 ? 0 : rect.y;
//    int x2 = rect.x + rect.width - 1;
//    int y2 = rect.y + rect.height - 1;
//    x2 = x2 < 0 ? 0 : x2;
//    y2 = y2 < 0 ? 0 : y2;
//    x2 = (x2 > frame_color.cols - 1) ? frame_color.cols - 1 : x2;
//    y2 = (y2 > frame_color.rows - 1) ? frame_color.rows - 1 : y2;
//    cv::Rect r(cv::Point(x1, y1), cv::Point(x2, y2));
//    std::vector<float> feat = feat_extracter->get_feat(frame_color, r, nullptr);
  std::vector<float> feat;
  return feat;
}

float CalcSimilarity(const vector<float> *feat1, const vector<float> *feat2) {
  if (feat1->size() != feat2->size()) {
    cout << "feat1 != feat2" << endl;
    return 0.0;
  }
  float similarity = 0.0;
  for (size_t i = 0; i < feat1->size(); i++) {
    similarity += (*feat1)[i] * (*feat2)[i];
  }
  return similarity;
}

vector<float> CombineFeature(const vector<float> &feat_new, const vector<float> &feat_old, float aratio) {
  vector<float> feature_combine;
  for (size_t i = 0; i < feat_new.size(); i++) {
    float val = aratio * feat_new[i] + (1 - aratio) * feat_old[i];
    feature_combine.push_back(val);
  }
  float feat_norm = 0.0;
  for (size_t i = 0; i < feature_combine.size(); i++) {
    feat_norm += feature_combine[i] * feature_combine[i];
  }
  feat_norm = sqrt(feat_norm);
  for (size_t i = 0; i < feature_combine.size(); i++) {
    feature_combine[i] = feature_combine[i] / feat_norm;
  }
  return feature_combine;
}
