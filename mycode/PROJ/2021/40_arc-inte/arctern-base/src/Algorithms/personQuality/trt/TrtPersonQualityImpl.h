
//
// Created by Admin on 2021/5/16.
//

#ifndef ARCTERN_BASE_TRTPERSONQUALITYIMPL_H
#define ARCTERN_BASE_TRTPERSONQUALITYIMPL_H
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/personQuality/personQuality.h"
using namespace std;
namespace arctern{
class TrtPersonQualityImpl: public ArcternAlgorithmEx{
 public:
  TrtPersonQualityImpl();

  virtual ~TrtPersonQualityImpl();



  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p,
                                     int startId,
                                     int endId,
                                     vector<arctern::OutputTensor<float>> &outputs) override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

 private:
  float BBOX_EXTSACL_TOP = 0;
  int FACEBACK_BRANCH_INDEX = -1;
  int PERSON_CLS_BRANCH_INDEX = -1;
  int RES_BRANCH_INDEX = 0;
  bool m_bGrayInput;
};
}

#endif //ARCTERN_BASE_TRTPERSONQUALITYIMPL_H
