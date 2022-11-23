/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  create:         2020.08.14
 *  email:          chen.gaohua@intellif.com
 *  Last modified:  2021.06.04 by kuanghongshen
 ****************************************************************************/

#pragma once
#include "src/Base/arcternalgorithm.h"
#include "faceDetectTiny.h"
#include "src/Base/YoloV3.h"
#include "src/Base/yoloV5.h"
#include <memory>

namespace arctern {

class faceDetectTinyImpl : public ArcternAlgorithm {
 public:
  faceDetectTinyImpl();
  ~faceDetectTinyImpl() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:
  int GetConfigInfo(arctern::MxnetModel *model) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float>> &outputs) override;

 protected:
  bool m_bNormalized ;
  std::unique_ptr<YoloV3> yolo_v_3_;
  std::unique_ptr<YoloV5> yolo_v_5_;
  bool square_face_;
  float exth_scale_face_ = 0;
  float extw_scale_face_ = 0;
  int m_version = 3;
};

}
