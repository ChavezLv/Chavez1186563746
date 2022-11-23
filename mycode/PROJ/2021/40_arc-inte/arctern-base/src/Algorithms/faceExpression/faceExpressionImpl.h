/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEEXPRESSION_FACEEXPRESSIONIMPL_H_
#define SRC_ALGORITHMS_FACEEXPRESSION_FACEEXPRESSIONIMPL_H_
#include <vector>
#include <utility>
#include "src/Base/arcternalgorithm.h"
#include "src/Algorithms/faceExpression/faceExpression.h"

namespace arctern {
class FaceExpressionImpl : public ArcternAlgorithm {
 public:
  FaceExpressionImpl();
  ~FaceExpressionImpl() override;
  void InitPreprocessorParam() override;
  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  std::pair<int, float> calcResult(const float *data, size_t num_class);

 protected:

  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEEXPRESSION_FACEEXPRESSIONIMPL_H_
