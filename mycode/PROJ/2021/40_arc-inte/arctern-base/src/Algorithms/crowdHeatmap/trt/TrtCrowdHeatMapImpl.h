//
// Created by Admin on 2021/5/16.
//

#ifndef ARCTERN_BASE_TRTCROWDHEATMAPIMPL_H
#define ARCTERN_BASE_TRTCROWDHEATMAPIMPL_H
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/crowdHeatmap/crowdHeatmap.h"
using namespace std;
namespace arctern {
class TrtCrowdHeatMapImpl : public ArcternAlgorithmEx {
 public:
  TrtCrowdHeatMapImpl();

  virtual ~TrtCrowdHeatMapImpl();

  virtual void InitPreprocessorParam() override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;



 private:


  std::array<float, 4> m_extScale;
};
}

#endif //ARCTERN_BASE_TRTCROWDHEATMAPIMPL_H
