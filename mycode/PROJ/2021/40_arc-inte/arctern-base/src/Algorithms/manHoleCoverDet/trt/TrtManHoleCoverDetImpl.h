//
// Created by lvchaolin on 2021/12/20.
//

#ifndef __ARCTERN_BASE_TRTMANHOLECOVERDETIMPL_H__
#define __ARCTERN_BASE_TRTMANHOLECOVERDETIMPL_H__

#include "../manHoleCoverDet.h"

#include "src/common/detectUtil.h"
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

#include "src/Base/YoloV3.h"
#include "src/Base/yoloV5.h"
using namespace std;

namespace arctern {

class TrtManHoleCoverDetectImpl : public ArcternAlgorithmEx {
 public:
  TrtManHoleCoverDetectImpl();

  virtual ~TrtManHoleCoverDetectImpl();

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;


  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;


  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual void InitPreprocessorParam() override;

 protected:
  vector<arctern::prior_box> GenerateBox();

  void InitPostProcessParam(int img_width,
                           int img_height,
                           std::vector<float> &v_scale_h,
                           std::vector<float> &v_scale_w);



 private:
  unique_ptr<YoloV3> m_yolo_v_3;
  bool m_bSquareFace;
  int m_version;
  float m_extHeightScaleFace = 0.0;
  float m_extWidthScaleFace = 0.0;

};
}

#endif //__ARCTERN_BASE_TRTMANHOLECOVERDETIMPL_H__
