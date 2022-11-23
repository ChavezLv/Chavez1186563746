//
// Created by Admin on 2021/6/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEHEADWEAR_TRT_TRTFACEHEADWEARIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEHEADWEAR_TRT_TRTFACEHEADWEARIMPL_H_
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"
#include "src/common/trt/TrtAlgorithmUtil.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/faceHeadwear/faceHeadwear.h"

using namespace std;
namespace arctern{
class TrtFaceHeadwearImpl: public ArcternAlgorithmEx{
 public:
  TrtFaceHeadwearImpl();

  virtual ~TrtFaceHeadwearImpl();

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId,vector<arctern::OutputTensor<float>> &outputs) override;



 private:
  pair<int,float> CalculateResult(const float *data, int step);

 protected:
  std::array<float, 4> m_extScale;
};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLASS_TRT_TRTFACEGLASSIMPL_H_
