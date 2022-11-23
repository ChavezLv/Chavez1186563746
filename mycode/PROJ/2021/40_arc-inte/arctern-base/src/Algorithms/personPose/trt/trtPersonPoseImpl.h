//
// Created by chengaohua on 2021/7/6.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONPOSE_TRT_TRTPERSONPOSEIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONPOSE_TRT_TRTPERSONPOSEIMPL_H_
#include "../personPose.h"
#include "src/Base/trt/ArcternAlgorithmEx.h"

namespace arctern {
class TrtPersonPoseImpl : public ArcternAlgorithmEx {
 public:
  TrtPersonPoseImpl();
  ~TrtPersonPoseImpl() override;

  ErrorType Process(const RunParameter *p,Result *r) override;
  void InitPreprocessorParam() override;
  ErrorType GetModelsConfigInfo(const MxnetModels *models) override;
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId, vector<arctern::OutputTensor<float>> &outputs) override;
  ArcternRect LocalGetROI(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;


 private:
  float bboxExtScalTop_ = 1.25f;
  std::vector<cv::Rect> upBBoxes_;

};
}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONPOSE_TRT_TRTPERSONPOSEIMPL_H_
