/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_PERSONFEAT_PERSONFEATIMPL_H_
#define SRC_ALGORITHMS_PERSONFEAT_PERSONFEATIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "common/arcterndefs_config_analyzer.h"
#include "personFeat.h"

namespace arctern {
class PersonFeatImpl : public ArcternAlgorithm {
 public:
  PersonFeatImpl();

  ~PersonFeatImpl() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 private:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  std::vector<uint8> encryptFeature(const std::vector<float> &feat);
 protected:
  bool isNormaliized_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_PERSONFEAT_PERSONFEATIMPL_H_
