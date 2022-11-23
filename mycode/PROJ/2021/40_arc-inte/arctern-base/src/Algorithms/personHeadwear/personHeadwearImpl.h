/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_PERSONHEADWEAR_PERSONHEADWEARIMPL_H_
#define SRC_ALGORITHMS_PERSONHEADWEAR_PERSONHEADWEARIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "personHeadwear.h"

namespace arctern {
class PersonHeadwearImpl : public ArcternAlgorithm {
 public:
  PersonHeadwearImpl();

  ~PersonHeadwearImpl() override;

  /// ErrorType Process(const RunParameter *p,Result *r) override;
  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
 private:

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
 protected:

  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_PERSONHEADWEAR_PERSONHEADWEARIMPL_H_
