/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         lvchaolin
 *  create:         2021.12.10
 *  email:          lv.chaolin@intellif.com
 ****************************************************************************/

#pragma once
#include "bannerDet.h"

#include "src/Base/YoloV3.h"
#include "src/Base/arcternalgorithm.h"

namespace arctern {

class BannerDetImpl final: public  ArcternAlgorithm{
public:
  BannerDetImpl();
  ~BannerDetImpl() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

private:
  int GetConfigInfo(arctern::MxnetModel *model) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float>> &outputs) override;
private:
  bool m_bNormalized;
  std::unique_ptr<YoloV3> m_yolo_v_3;

  bool m_square_face;
  float m_exth_scale_face;
  float m_extw_scale_face;

};

} // namespace arctern
