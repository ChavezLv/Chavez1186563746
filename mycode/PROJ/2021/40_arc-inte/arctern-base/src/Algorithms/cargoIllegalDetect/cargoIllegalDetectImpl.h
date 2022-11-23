/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         lvchaolin
 *  create:         2021.12.20
 *  email:          lv.chaolin@intellif.com
 ****************************************************************************/

#pragma once

#include "src/Base/YoloV4.h"
#include "src/Base/arcternalgorithm.h"

namespace arctern {

class CargoIllegalDetectImpl final: public  ArcternAlgorithm{
public:
  CargoIllegalDetectImpl();
  ~CargoIllegalDetectImpl() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

private:
  int GetConfigInfo(arctern::MxnetModel *model) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float>> &outputs) override;
private:

  std::unique_ptr<YoloV4> m_yolo_v_4;
};

} // namespace arctern
