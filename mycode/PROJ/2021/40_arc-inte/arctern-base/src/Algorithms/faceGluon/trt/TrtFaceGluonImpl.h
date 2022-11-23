//
// Created by Admin on 2019/6/9.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_TRT_TRTFACEGLUONIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_TRT_TRTFACEGLUONIMPL_H_
#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Algorithms/faceGluon/faceGluon.h"
#include "../utils/faceGluonUtil.h"
namespace arctern {
class TrtFaceGluonImpl : public ArcternAlgorithmEx {
 public:
  TrtFaceGluonImpl();
  ~TrtFaceGluonImpl() override;

  ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  void InitPreprocessorParam() override;

  /*ErrorType LocalPreProcess(const RunParameter *p, const int &idx) override;*/
  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  ArcternRect LocalGetROI(const RunParameter *p, int idx) override;


 private:
  string m_version;
  bool m_bNeedLandmark;
  bool m_bMaskWipe = false;
  gluon::LocalPreProcessV m_type;
  std::vector<ArcternPoint> m_lmk;
  std::array<float, 2> m_extHeightScale;
  std::array<float, 2> m_extWidthScale;
};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_TRT_TRTFACEGLUONIMPL_H_
