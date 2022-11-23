//
// Created by chengaohua on 2021/6/24.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONHEADWEAR_TRT_TRTPERSONHEADWEARIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONHEADWEAR_TRT_TRTPERSONHEADWEARIMPL_H_

#include "src/Base/trt/ArcternAlgorithmEx.h"
namespace arctern {
class TrtPersonHeadwearImpl : public ArcternAlgorithmEx {
 public:
  TrtPersonHeadwearImpl();
  ~TrtPersonHeadwearImpl() override;
  ErrorType GetModelsConfigInfo(const MxnetModels *models) override;
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  ArcternRect LocalGetROI(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;
 private:
  std::array<float, 4> extScale_;
};

}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONHEADWEAR_TRT_TRTPERSONHEADWEARIMPL_H_
