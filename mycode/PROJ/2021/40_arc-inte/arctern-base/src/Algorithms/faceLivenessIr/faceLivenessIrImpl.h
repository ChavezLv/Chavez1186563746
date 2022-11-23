/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACELIVENESSIR_FACELIVENESSIRIMPL_H_
#define SRC_ALGORITHMS_FACELIVENESSIR_FACELIVENESSIRIMPL_H_

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "faceLivenessIr.h"

namespace arctern {
class FaceLivenessIrImpl : public ArcternAlgorithm {
 public:
  FaceLivenessIrImpl();

  ~FaceLivenessIrImpl() noexcept override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  ArcternRect getFace(int width, int height, const cv::Rect &rect);

 protected:
  bool isNormaliized_;  /// < hisi not need normalized
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACELIVENESSIR_FACELIVENESSIRIMPL_H_
