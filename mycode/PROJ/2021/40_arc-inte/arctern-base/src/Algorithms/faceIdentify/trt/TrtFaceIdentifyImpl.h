// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  create:         2021.06.05
// *  email:          kuang.hongshen@intellif.com
// *  modified:
// ****************************************************************************

#ifndef SRC_ALGORITHMS_FACEIDENTIFYIMPL_FACEIDENTIFYIMPL_H_
#define SRC_ALGORITHMS_FACEIDENTIFYIMPL_FACEIDENTIFYIMPL_H_
#include "../../../Base/trt/ArcternAlgorithmEx.h"
#include "../../../Engin/src/trt/trtEngineManager.h"
#include "../../../Processors/NppPreprocessorEx.h"
#include "src/Algorithms/faceIdentify/faceIdentify.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/imageConvert.h"

namespace arctern {
class TrtFaceIdentifyImpl final : public ArcternAlgorithmEx {
public:
  TrtFaceIdentifyImpl();
  ~TrtFaceIdentifyImpl();

private:
  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;


  std::pair<int, float> calcType(const float *p_data, const size_t class_num);

  cv::Rect getFace(const int width, const int height, const cv::Rect bbox);

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::OutputTensor<float>> &outputs);

  void InitEngines() {}

  ErrorType GetModelsConfigInfo(const MxnetModels *models);

private:
  std::array<float, 4> m_aExtScale;
};
} // namespace arctern
#endif
