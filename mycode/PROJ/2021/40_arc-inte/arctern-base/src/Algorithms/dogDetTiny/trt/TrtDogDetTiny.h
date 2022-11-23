//
// Created by Admin on 2019/6/15.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_DOGDETTINY_TRT_TRTDOGDETTINY_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_DOGDETTINY_TRT_TRTDOGDETTINY_H_
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"
#include "src/common/detectUtil.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"

#include "src/Engin/src/mxnet/MxnetEngineManager.h"
#include "src/Engin/src/mxnet/mxnetengin.h"
#include "src/Engin/src/trt/trtEngineManager.h"

#include "src/Algorithms/dogDetTiny/dogDetTiny.h"
#include "src/Base/YoloV3.h"
#include "src/Base/yoloV5.h"
using namespace std;
namespace arctern{

class TrtDogDetTinyImpl: public ArcternAlgorithmEx{
 public:
  TrtDogDetTinyImpl();

  virtual ~TrtDogDetTinyImpl();

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId,vector<arctern::OutputTensor<float>> &outputs) override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual void InitPreprocessorParam() override;

 private:
  unique_ptr<YoloV3> m_yolo_v_3;
  bool m_bSquareFace;
  float exth_scale_face_ = 0.0;
  float extw_scale_face_ = 0.0;
};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_DOGDETTINY_TRT_TRTDOGDETTINY_H_
