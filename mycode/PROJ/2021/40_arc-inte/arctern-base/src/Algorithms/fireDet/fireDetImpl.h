//
// Created by Admin on 2021/12/22.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FIREDET_FIREDETIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FIREDET_FIREDETIMPL_H_

#pragma once

#include "src/Base/YoloV4.h"
#include "src/Base/arcternalgorithm.h"

namespace arctern {

class FireDetImpl final: public  ArcternAlgorithm{
 public:
  FireDetImpl();
  ~FireDetImpl() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 private:
  int GetConfigInfo(arctern::MxnetModel *model) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float>> &outputs) override;
 private:

  std::unique_ptr<YoloV4> m_yolo_v_4;
  std::string type="";
};

} // namespace arctern


#endif //ARCTERN_BASE_SRC_ALGORITHMS_FIREDET_FIREDETIMPL_H_
