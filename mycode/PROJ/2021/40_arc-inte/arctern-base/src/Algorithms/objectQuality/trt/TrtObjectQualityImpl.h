// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  create:         2021.06.05
// *  email:          kuang.hongshen@intellif.com
// *  modified:
// ****************************************************************************

#ifndef SRC_ALGORITHMS_TRTOBJECTQUALITY_TRTOBJECTQUALITY_H_
#define SRC_ALGORITHMS_TRTOBJECTQUALITY_TRTOBJECTQUALITY_H_
#include "../../../Base/trt/ArcternAlgorithmEx.h"
#include "../../../Engin/src/trt/trtEngineManager.h"
#include "../../../Processors/NppPreprocessorEx.h"
#include "src/Algorithms/objectQuality/objectQuality.h"
#include "src/Base/arcternalgorithm.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/imageConvert.h"
#include "src/common/tensor.h"

namespace arctern {
class TrtObjectQualityImpl final : public ArcternAlgorithmEx {
public:
  TrtObjectQualityImpl();
  ~TrtObjectQualityImpl();

private:
  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::OutputTensor<float>> &outputs);

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

  void InitEngines() {}

  ErrorType
  GetModelsConfigInfo(const MxnetModels *models);

private:
  int m_iBranchNum;
};
} // namespace arctern
#endif
