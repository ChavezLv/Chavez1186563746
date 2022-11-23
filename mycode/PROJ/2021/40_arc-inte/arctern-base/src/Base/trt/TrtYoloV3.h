/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  modified:       2020.08.14
 *  email:          chen.gaohua@intellif.com
 *  Last modified:  @2021.06.04  by kuanghongshen
 ****************************************************************************/

#ifndef ARCTERN_TRTYOLOV3_H
#define ARCTERN_TRTYOLOV3_H

#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"
#include "src/common/detectUtil.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"

#include "src/Engin/src/mxnet/MxnetEngineManager.h"
#include "src/Engin/src/mxnet/mxnetengin.h"
#include "src/Engin/src/trt/trtEngineManager.h"

namespace arctern {

struct detection {
  ArcternRect bbox;
  int classes;
  float *prob;
  float *mask;
  float objectness;
  float top_occluded_prob;
  float bottom_occluded_prob;
  int sort_class;
};
class TrtYoloV3 : public ArcternAlgorithmEx {
 public:
  TrtYoloV3();

  virtual ~TrtYoloV3();

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual void InitPreprocessorParam() override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId, vector<arctern::OutputTensor<float>> &outputs) override;

  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

 public:
  void TransformGrid(float *prediction);
  vector<detection> PackValidDetections(float *prediction);
  void DoNmsSort(detection *dets, int total);
  static int NmsComparator(const void *pa, const void *pb);
  Vbbox PackResults(int oriWidth, int oriHeight, const std::vector<detection> &detections) const;

 public:
  int m_classNum;
  int m_inputDim;
  int m_inputDim2;
  float m_nmsThresh;
  bool m_bDetPersonFlag;
  bool m_bTinyYolov3Flag;
  std::vector<std::vector<int> > m_vAnchors;

  bool m_bSquareFace;
  int m_strides;
  int m_boxs;
  std::vector<std::vector<int>> m_vStrides;
  std::vector<std::vector<int> > m_vStep;

  float exth_scale_face_ = 0.0;
  float extw_scale_face_ = 0.0;
  float m_configThresh;
};


} // namespace arctern

#endif
