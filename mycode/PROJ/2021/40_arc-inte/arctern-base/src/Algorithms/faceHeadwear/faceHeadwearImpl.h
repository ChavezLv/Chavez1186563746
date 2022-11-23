/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEHEADWEAR_FACEHEADWEARIMPL_H_
#define SRC_ALGORITHMS_FACEHEADWEAR_FACEHEADWEARIMPL_H_
#include <vector>
#include <utility>
#include "src/Base/arcternalgorithm.h"
#include "./faceHeadwear.h"

namespace arctern {
class FaceHeadwearImpl : public ArcternAlgorithm {
 public:
  FaceHeadwearImpl();

  ~FaceHeadwearImpl() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}
 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;

  std::pair<int, float> calcResult(const float *data, size_t num_class);

 protected:
  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEHEADWEAR_FACEHEADWEARIMPL_H_
