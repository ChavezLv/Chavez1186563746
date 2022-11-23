//
// Created by Admin on 2021/6/21.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_TRT_TRTPERSONCOMPLETENESSIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_TRT_TRTPERSONCOMPLETENESSIMPL_H_
#include <common/detectUtil.h>
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/Engin/src/mxnet/MxnetEngineManager.h"
#include "src/Engin/src/mxnet/mxnetengin.h"
#include "../../../Engin/src/trt/trtEngineManager.h"
#include "src/common/common.h"

#include "../personCompleteness.h"
namespace arctern {

class TrtPersonCompletenessImpl : public ArcternAlgorithmEx {
 public:
  TrtPersonCompletenessImpl();
  virtual ~TrtPersonCompletenessImpl();

 protected:


  virtual void InitPreprocessorParam() override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

 protected:
  ArcternRect GetROI(const int &img_width,
                    const int &img_height,
                    const ArcternRect &bbox,
                    const std::array<float, 4> ext_scale) const;
 private:
  std::array<float, 4> m_extScale;


};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_TRT_TRTPERSONCOMPLETENESSIMPL_H_
