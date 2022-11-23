#ifndef ARCTERN_YOLOV4_H
#define ARCTERN_YOLOV4_H
#include "common/arcterndefs_config_analyzer.h"
#include "common/arcterndefs_generated.h"
#include "common/detectUtil.h"
#include "common/file_buffer.h"
#include "common/tensor.h"
#include "opencv2/opencv.hpp"
#include <vector>
#include "YoloV3.h"
namespace arctern {
class YoloV4 final {
public:
  YoloV4();

   ~YoloV4();

   int getConfigInfo(arctern::MxnetModel *model);
   int getConfigInfo(arctern::TrtModel *model);

   int getNetH();
   int getNetW();

  Vbbox postProcess(float * data, int width, int height, float thresh);
  Vbbox postProcess2(float * data, int width, int height, float thresh);
 private:
  void do_nms_sort(detection *dets, int total);
  void transform_grid(float *prediction);
  void transform_grid2(float *prediction);
  Vbbox pack_results(int oriWidth, int oriHeight, std::vector<detection> &detections);
  
  std::vector<detection> pack_valid_detections(float *prediction);
public:
  ///> unify to output square boxes


 public:
  int width_ = 608;
  int height_ = 608;
  float thresh_ = 0.001;
  float nms_thresh = 0.45;
  int num_classes = 1;
};
} // namespace arctern
#endif
