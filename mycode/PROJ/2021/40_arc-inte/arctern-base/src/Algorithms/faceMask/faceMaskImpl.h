/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEMASK_FACEMASKIMPL_H_
#define SRC_ALGORITHMS_FACEMASK_FACEMASKIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"


namespace arctern {
class FaceMaskImpl : public ArcternAlgorithm {
 public:
  FaceMaskImpl();

  ~FaceMaskImpl() override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  virtual std::pair<int, float> calc_mask(const float *data, size_t num_class) ;

 protected:
  std::array<float, 4> extScale_;
  bool isNormaliized_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEMASK_FACEMASKIMPL_H_
