/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEBANGS_FACEBANGSIMPL_H_
#define SRC_ALGORITHMS_FACEBANGS_FACEBANGSIMPL_H_
#include <vector>
#include <utility>
#include "src/Base/arcternalgorithm.h"
#include "./faceBangs.h"

namespace arctern {
class FaceBangsImpl : public ArcternAlgorithm {
 public:
  FaceBangsImpl();

  ~FaceBangsImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;


  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:

//  cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter *p, int idx) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  std::pair<int, float> calcResult(const float *data, size_t num_class);

 protected:


  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEBANGS_FACEBANGSIMPL_H_
