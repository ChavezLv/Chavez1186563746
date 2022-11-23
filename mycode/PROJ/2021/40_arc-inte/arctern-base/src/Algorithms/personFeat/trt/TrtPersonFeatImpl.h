//
// Created by chengaohua on 2021/6/22.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONFEAT_TRT_TRTPERSONFEATIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONFEAT_TRT_TRTPERSONFEATIMPL_H_
#include "src/Base/trt/ArcternAlgorithmEx.h"

namespace arctern {
class TrtPersonFeatImpl : public ArcternAlgorithmEx {
 public:
  TrtPersonFeatImpl();
  ~TrtPersonFeatImpl() override;


  ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

  std::vector<uint8> encryptFeature(const std::vector<float> &feat);

  void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;
};

}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONFEAT_TRT_TRTPERSONFEATIMPL_H_
