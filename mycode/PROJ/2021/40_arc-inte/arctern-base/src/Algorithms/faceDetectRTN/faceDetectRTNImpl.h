/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEDETECTRTN_FACEDETECTRTNIMPL_H_
#define SRC_ALGORITHMS_FACEDETECTRTN_FACEDETECTRTNIMPL_H_
#include <vector>
#include "log/logger.h"
#include "src/Base/algorithmbase.h"
#include "faceDetectRTN.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/detectUtil.h"
#include "src/common/tensor.h"
#include "src/Base/arcternalgorithm.h"
//#include "src/Processors/Algorithms/detpreprocess.h"

namespace arctern {

class FaceDetectRTNImpl : public ArcternAlgorithm {
 public:
  FaceDetectRTNImpl();
  ~FaceDetectRTNImpl() noexcept override;
  //ErrorType Init(const InitParameter *p) override;
  //void Release() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:

  std::vector<prior_box> generate_box();
  void initPostProcessParam(int img_width, int img_height,
                            std::vector<float> &v_scale_h,
                            std::vector<float> &v_scale_w);
  Vbbox decode_box_landmark(float *locations, float *scores,
                            float *landmarks, float *quality,
                            float scale_h, float scale_w, int stride);
  Vbbox nms(Vbbox &keep_bbox, float thresh);

 protected:
  int num_output_ = 3;
  int num_landmark_ = 5;    ///  < face landmark num
  //  float conf_thresh;
  float nms_thresh_ = 0.45;   ///  < nms thresh
  bool square_face_ = true;
  float exth_scale_face_ = 0.0;
  float extw_scale_face_ = 0.0;
  bool using_landmark_;  ///  output landmark
  bool using_quality_;   ///  output quality
  float conf_thresh_;
  std::vector<float> steps_ = {8, 16, 32};
  std::vector<float> variance_ = {0.1, 0.2};
  std::vector<std::vector<int>> feature_maps_;
  std::vector<prior_box> anchors_;
  std::vector<std::vector<float>> min_sizes_ = {{16, 32}, {64, 128}, {256, 512}};

};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEDETECTRTN_FACEDETECTRTNIMPL_H_
