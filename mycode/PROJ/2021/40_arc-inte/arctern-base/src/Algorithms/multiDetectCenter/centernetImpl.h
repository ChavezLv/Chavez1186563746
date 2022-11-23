/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.26
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_MULTIDETECTCENTER_CENTERNETIMPL_H_
#define SRC_ALGORITHMS_MULTIDETECTCENTER_CENTERNETIMPL_H_
#include <vector>
#include "opencv2/opencv.hpp"
#include "src/Base/arcternalgorithm.h"
#include "common/file_buffer.h"
#include "common/detectUtil.h"
#include "common/tensor.h"
#include "common/arcterndefs_generated.h"
#include "common/arcterndefs_config_analyzer.h"
#include "Base/algorithmbase.h"
#include "multiDetectCenter.h"

namespace arctern {

class CenterNet : public ArcternAlgorithm {
 public:
  CenterNet();

  ~CenterNet() override;

  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  void InitPreprocessorParam() override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
  ErrorType CheckRunParam(const RunParameter *p) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

 protected:
  bool initMemberVariable();
  void initPostProcessParam(int img_width,
                            int img_height,
                            const cv::Size &osize,
                            std::vector<int> &pad_w,
                            std::vector<int> &pad_h,
                            std::vector<float> &img_ratio);

 public:
  bool square_rect_ = false;  // unify to output square boxes
  float exth_scale_rect_ = 0.0;
  float extw_scale_rect_ = 0.0;

 protected:
  int input_dim_w;
  int input_dim_h;
  int output_dim_w;
  int output_dim_h;
  float hm_ratio_w_;
  float hm_ratio_h_;
  bool flag_quality;
  bool flag_ind_wh;
  std::map<int, int> cls_project;
  std::vector<int> vec_cls_project;
  std::string version_;

};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_MULTIDETECTCENTER_CENTERNETIMPL_H_
