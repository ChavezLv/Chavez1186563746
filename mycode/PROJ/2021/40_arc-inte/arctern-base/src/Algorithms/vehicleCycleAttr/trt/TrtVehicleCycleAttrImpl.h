//
// Created by Admin on 2021/6/21.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEATTR_TRT_TRTVEHICLECYCLEATTRIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEATTR_TRT_TRTVEHICLECYCLEATTRIMPL_H_
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

#include "../vehicleCycleAttr.h"
namespace arctern {

class TrtVehicleCycleAttrImpl : public ArcternAlgorithmEx {
 public:
  TrtVehicleCycleAttrImpl();
  virtual ~TrtVehicleCycleAttrImpl();

 protected:


  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

 private:
  pair<int,float> CalculateResult(const float *data, int step);
  ArcternRect GetVehicle(const int& width, const int& height, const ArcternRect &bbox);
  std::array<float, 4> m_extScale;

};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEATTR_TRT_TRTVEHICLECYCLEATTRIMPL_H_
