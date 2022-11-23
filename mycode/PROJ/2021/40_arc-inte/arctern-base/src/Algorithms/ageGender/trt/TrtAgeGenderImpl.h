//
// Created by Admin on 2021/6/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_TRT_TRTAGEGENDERIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_TRT_TRTAGEGENDERIMPL_H_
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"
#include "src/common/trt/TrtAlgorithmUtil.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/ageGender/ageGender.h"

using namespace std;
namespace arctern{
class TrtAgeGenderImpl:public ArcternAlgorithmEx{
 public:
  TrtAgeGenderImpl();

  virtual ~TrtAgeGenderImpl();



  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId,vector<arctern::OutputTensor<float>> &outputs) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

 private:
  pair<int,float> CalculateResult(const float *data, int step);
  ArcternRect GetFace(int width, int height, const ArcternRect &bbox);
 protected:
  ConfigParameter m_config;
  std::array<float, 4> m_extScale;
};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_AGEGENDER_TRT_TRTAGEGENDERIMPL_H_
