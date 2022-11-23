//
// Created by Admin on 2021/6/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACIALLANDMARK_TRT_TRTFACIALLANDMARKIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACIALLANDMARK_TRT_TRTFACIALLANDMARKIMPL_H_
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"
#include "src/common/trt/TrtAlgorithmUtil.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/facialLandmark/facialLandmark.h"

using namespace std;
namespace arctern{
class TrtFacialLandmarkImpl: public ArcternAlgorithmEx{
 public:
  TrtFacialLandmarkImpl();

  virtual ~TrtFacialLandmarkImpl();



  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId,vector<arctern::OutputTensor<float>> &outputs) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

  virtual ErrorType Process(const RunParameter *p,Result *r) override;
 protected:
  int m_landmarkNum;
  std::vector<ArcternRect > m_vFaceRects;
};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLASS_TRT_TRTFACEGLASSIMPL_H_
