//
// Created by gh on 2021/8/26.
//

#ifndef ARCTERN_BASE_SRC_BASE_YOLOV5_H_
#define ARCTERN_BASE_SRC_BASE_YOLOV5_H_

#include "common/detectUtil.h"
#include "common/arcterndefs_config_analyzer.h"
#include "common/arcterndefs_generated.h"

namespace arctern {
class YoloV5 final  {
 public:
  YoloV5() = default;
  ~YoloV5() = default;

  std::vector<int> get_anchors(int net_grid);

  Vbbox postProcess(float * data0,float * data1, float * data2, int width, int height, float thresh);
  Vbbox postProcess2(float *data0, float *data1, float *data2, int width, int height, float thresh);

  int getConfigInfo(arctern::MxnetModel *model);

  int getNetH();

  int getNetW();

  void post(std::vector<Bbox>& bboxes, int w, int h);

  //注意此处的阈值是框和物体prob乘积的阈值
  void parse_yolov5(void* output_tensor,int net_grid, float cof_threshold, std::vector<Bbox>& o_bboxes);
  void doNMS(std::vector<Bbox>& bboxes, float nms_thresh);

 private:
  int input_dim_ = 640;
  int input_dim2_ = 640;
  float nms_thresh_ = 0.45;
};
}

#endif //ARCTERN_BASE_SRC_BASE_YOLOV5_H_
