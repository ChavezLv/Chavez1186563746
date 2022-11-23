/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  create:         2021.06.04
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#pragma once
#include "src/Base/YoloV3.h"
#include "dogDetTiny.h"
#include "src/Base/arcternalgorithm.h"

namespace arctern {

class dogDetTinyImpl : public  ArcternAlgorithm{
public:
  dogDetTinyImpl();
  ~dogDetTinyImpl() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
private:
  int GetConfigInfo(arctern::MxnetModel *model) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float>> &outputs) override;
private:
  bool m_bNormalized;
  std::unique_ptr<YoloV3> yolo_v_3_;
  bool square_face_;
  float exth_scale_face_;
  float extw_scale_face_;

};

} // namespace arctern
