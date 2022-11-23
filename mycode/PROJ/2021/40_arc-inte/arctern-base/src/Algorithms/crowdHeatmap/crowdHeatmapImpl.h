/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.18
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_CROWDHEADMAP_CROWDHEADMAPIMPL_H_
#define SRC_ALGORITHMS_CROWDHEADMAP_CROWDHEADMAPIMPL_H_

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "crowdHeatmap.h"

namespace arctern {
class CrowdHeatmapImpl : public ArcternAlgorithm {
 public:
  CrowdHeatmapImpl();

  ~CrowdHeatmapImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
  int GetConfigInfo(arctern::MxnetModel *model) override { return 0; };

 protected:
  // void initPreProcessor();

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_CROWDHEADMAP_CROWDHEADMAPIMPL_H_
