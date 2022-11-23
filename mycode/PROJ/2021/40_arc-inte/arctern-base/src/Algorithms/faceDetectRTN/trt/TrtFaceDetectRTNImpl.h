//
// Created by Admin on 2020/6/4.
//

#ifndef ARCTERN_BASE_TRTFACEDETECTRTNIMPL_H
#define ARCTERN_BASE_TRTFACEDETECTRTNIMPL_H
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

#include "../faceDetectRTN.h"
namespace arctern {

class TrtFaceDetectRTNImpl : public ArcternAlgorithmEx {
 public:
  TrtFaceDetectRTNImpl();
  virtual ~TrtFaceDetectRTNImpl();

 protected:

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  virtual void InitPreprocessorParam() override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;
 protected:
  vector<arctern::prior_box> GenerateBox();

  void InitPostProcessParam(int img_width,
                           int img_height,
                           std::vector<float> &v_scale_h,
                           std::vector<float> &v_scale_w);

  Vbbox DecodeBoxLandmark(float *locations,
                         float *scores, float *landmarks, float *quality,
                         float scale_h, float scale_w, int stride);

  Vbbox DoNmsSort(Vbbox &keep_bbox, float thresh);

 protected:
  int m_numOutput = 3;
  ///< face landmark num
  int m_numLandmark = 5;

  ///< nms thresh
  float m_nmsThresh = 0.45;
  ///< config thresh
  float m_confThresh;

  bool m_bSquareFace = true;

  ///  output landmark
  bool m_bUsingLandmark;
  ///  output quality
  bool m_bUsingQuality;

  std::vector<float> m_vSteps = {8, 16, 32};
  std::vector<float> m_vVariance = {0.1, 0.2};
  std::vector<std::vector<int>> m_vFeatureMaps;
  std::vector<prior_box> m_vAnchors;
  std::vector<std::vector<float>> m_vminSizes = {{16, 32},
                                                 {64, 128},
                                                 {256, 512}};

  float m_extHeightScaleFace = 0.0;
  float m_extWidthScaleFace = 0.0;

};
}

#endif //ARCTERN_BASE_TRTFACEDETECTRTNIMPL_H
