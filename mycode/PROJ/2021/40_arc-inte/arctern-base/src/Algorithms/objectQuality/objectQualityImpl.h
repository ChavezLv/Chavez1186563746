/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.03
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_PERSONQUALITY_PERSONQUALITYIMPL_H_
#define SRC_ALGORITHMS_PERSONQUALITY_PERSONQUALITYIMPL_H_

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "objectQuality.h"

namespace arctern {
class ObjectQualityImpl: public ArcternAlgorithm {
 public:
  ObjectQualityImpl();
  ~ObjectQualityImpl() override;
  int GetConfigInfo(arctern::MxnetModel *model) override;
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                            std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
 protected:
  int branchNum_ = 1;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_PERSONQUALITY_PERSONQUALITYIMPL_H_
