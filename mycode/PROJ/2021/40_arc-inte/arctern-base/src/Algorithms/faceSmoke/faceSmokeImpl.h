/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACESMOKE_FACESMOKEIMPL_H_
#define SRC_ALGORITHMS_FACESMOKE_FACESMOKEIMPL_H_
#include <vector>
#include <utility>
#include "src/Base/arcternalgorithm.h"
#include "faceSmoke.h"

namespace arctern {
class FaceSmokeImpl : public ArcternAlgorithm {
 public:
  FaceSmokeImpl();

  ~FaceSmokeImpl() override;

  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;
  std::pair<int, float> calcSmoke(const float *data, size_t num_class);
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACESMOKE_FACESMOKEIMPL_H_
