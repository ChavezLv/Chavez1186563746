//
// Created by Admin on 2021/6/21.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEEXPRESSION_TRT_TRTFACEEXPRESSIONIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEEXPRESSION_TRT_TRTFACEEXPRESSIONIMPL_H_
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

#include "../faceExpression.h"
namespace arctern {

class TrtFaceExpressionImpl : public ArcternAlgorithmEx {
 public:
  TrtFaceExpressionImpl();
  virtual ~TrtFaceExpressionImpl();

 protected:




  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual void InitPreprocessorParam() override ;

 private:
  pair<int,float> CalculateResult(const float *data, int step);

 protected:
  std::array<float, 4> m_extScale;
};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEEXPRESSION_TRT_TRTFACEEXPRESSIONIMPL_H_
