//
// Created by chengaohua on 2021/7/2.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEFEAT_TRT_TRTVEHICLECYCLEFEATIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEFEAT_TRT_TRTVEHICLECYCLEFEATIMPL_H_

#include "src/Base/trt/ArcternAlgorithmEx.h"

namespace arctern {
class TrtVehicleCycleFeatImpl : public ArcternAlgorithmEx {
 public:
  TrtVehicleCycleFeatImpl();
  ~TrtVehicleCycleFeatImpl() override;

  ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

  void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;


};

}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEFEAT_TRT_TRTVEHICLECYCLEFEATIMPL_H_
