//
// Created by Admin on 2021/6/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_MULTIDETECT_TRT_TRTMULTIDETECTIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_MULTIDETECT_TRT_TRTMULTIDETECTIMPL_H_
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"
#include "src/common/trt/TrtAlgorithmUtil.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "src/Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/multiDetectCenter/multiDetectCenter.h"

using namespace std;
namespace arctern{
class TrtMultiDetectCenterImpl: public ArcternAlgorithmEx{
 public:
  TrtMultiDetectCenterImpl();

  virtual ~TrtMultiDetectCenterImpl();

  virtual void InitPreprocessorParam() override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId,vector<arctern::OutputTensor<float>> &outputs) override;
 private:
  void initPostProcessParam(int img_width, int img_height, const cv::Size& osize, std::vector<int> &pad_w, std::vector<int> &pad_h, std::vector<float> &img_ratio);

 protected:
  bool square_rect_ = false;
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
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLASS_TRT_TRTFACEGLASSIMPL_H_
