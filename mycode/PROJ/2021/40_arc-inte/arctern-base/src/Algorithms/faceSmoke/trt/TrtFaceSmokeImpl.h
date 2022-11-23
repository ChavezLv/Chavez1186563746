//
// Created by chengaohua on 2021/6/24.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACESMOKE_TRT_TRTFACESMOKEIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACESMOKE_TRT_TRTFACESMOKEIMPL_H_

#include "src/Base/trt/ArcternAlgorithmEx.h"
namespace arctern {
class TrtFaceSmokeImpl : public ArcternAlgorithmEx {
 public:
  TrtFaceSmokeImpl();
  ~TrtFaceSmokeImpl() override;
  ErrorType GetModelsConfigInfo(const MxnetModels *models) override;
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  ArcternRect LocalGetROI(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACESMOKE_TRT_TRTFACESMOKEIMPL_H_
