//
// Created by Admin on 2021/6/21.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONATTR_TRT_TRTPERSONATTRIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONATTR_TRT_TRTPERSONATTRIMPL_H_
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

#include "../personAttr.h"
namespace arctern {

class TrtPersonAttrImpl : public ArcternAlgorithmEx {
 public:
  TrtPersonAttrImpl();
  virtual ~TrtPersonAttrImpl();

 protected:


  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

 private:
  pair<int,float> CalculateResult(const float *data, int step);

 protected:
  ArcternRect
  GetPerson(const int img_height,
            const int img_width,
            const ArcternRect &bbox,
            const std::array<float, 4> &ext_scale,
            const std::vector<float> &pad_scale);

    std::vector<float> get_padded_bbox(const ArcternRect &bbox,const std::vector<float> &pad_scale);
 private:
  std::array<float, 4> m_extScale;
  vector<float> m_vPaddingParam;
};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONATTR_TRT_TRTPERSONATTRIMPL_H_
