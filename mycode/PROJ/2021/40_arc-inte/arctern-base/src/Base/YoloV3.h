/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  modified:       2020.08.14
 *  email:          chen.gaohua@intellif.com
 *  Last modified:  @2021.06.04  by kuanghongshen
 ****************************************************************************/

#ifndef ARCTERN_YOLOV3_H
#define ARCTERN_YOLOV3_H
#include "common/arcterndefs_config_analyzer.h"
#include "common/arcterndefs_generated.h"
#include "common/detectUtil.h"
#include "common/file_buffer.h"
#include "common/tensor.h"
#include "opencv2/opencv.hpp"
#include <vector>
namespace arctern {
struct detection {
  ArcternRect bbox;
  int classes;
  float *prob;
  float *mask;
  float objectness;
  float top_occluded_prob;
  float bottom_occluded_prob;
  int sort_class;
};
inline int nms_comparator(const void *pa, const void *pb) {
  detection a = *reinterpret_cast<const detection *>(pa);
  detection b = *reinterpret_cast<const detection *>(pb);
  float diff = 0;
  if (b.sort_class >= 0) {
    diff = a.prob[b.sort_class] - b.prob[b.sort_class];
  } else {
    diff = a.objectness - b.objectness;
  }
  if (diff < 0) {
    return 1;
  } else if (diff > 0) {
    return -1;
  }
  return 0;
}
class YoloV3 final {
public:
  YoloV3();
   ~YoloV3();
   int getConfigInfo(arctern::MxnetModel *model);
   int getConfigInfo(arctern::TrtModel *model);

   int getNetH();

   int getNetW();

   void postProcess(float * data, int width, int height, float thresh,Vbbox &vbbox);
   void postProcess2(float *data, int width, int height, float thresh, Vbbox &vbbox);
   void postProcess3(float *data, int width, int height, float thresh, Vbbox &vbbox);
 private:

  bool initMemberVariable();
  void do_nms_sort(detection *dets, int total);
  void transform_grid(float *prediction);
  Vbbox pack_results(int oriWidth, int oriHeight,
                     const std::vector<detection> &detections) const;
  void pack_results(int oriWidth, int oriHeight,
                    const std::vector<detection> &detections,
                    Vbbox &boxes ) const;
  Vbbox pack_results2(int oriWidth, int oriHeight, const std::vector<detection> &detections);
  Vbbox pack_results3(int oriWidth, int oriHeight,const std::vector<detection>& detections) ;
  
  void pack_valid_detections(float *prediction,std::vector<detection> &detections);
  std::vector<detection> pack_valid_detections(float *prediction);
 public:
  int num_classes_;
  int input_dim_;
  int input_dim2_;
  int nstrides_;
  int nboxes_;
  float config_thresh;
  float nms_thresh_;
  bool det_person_flag_;
  bool tiny_yolov3_flag_;
  std::vector<std::vector<int>> anchors_;
  std::vector<std::vector<int>> step_;
  std::vector<std::vector<int>> strides_;
};
} // namespace arctern
#endif
