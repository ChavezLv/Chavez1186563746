/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.24
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEBASICMULTIATTR_FACEBASICMULTIATTRIMPL_H_
#define SRC_ALGORITHMS_FACEBASICMULTIATTR_FACEBASICMULTIATTRIMPL_H_
#include <vector>
#include <utility>
#include "src/Base/arcternalgorithm.h"
#include "common/arcterndefs_config_analyzer.h"
#include "./faceBasicMultiAttr.h"

namespace arctern {
class FaceBasicMultiAttrImpl : public ArcternAlgorithm {
 public:
  FaceBasicMultiAttrImpl();

  ~FaceBasicMultiAttrImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
 // void initPreProcessor();
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  //cv::Mat LocalGetFace(const cv::Mat & image, const RunParameter * p, int idx) override;

  std::pair<int, float> calcResult(const float *data, size_t num_class);
  std::pair<int, float> calc_age(const float *data, size_t num_class);

 protected:

  std::array<float, 4> extScale_;
  int age_idx_ = 1;
  int task_num_ = 5;
  int age_range_ = 86;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEBASICMULTIATTR_FACEBASICMULTIATTRIMPL_H_
