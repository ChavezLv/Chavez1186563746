/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.19
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACIALLANDMARK_FACIALLANDMARKIMPL_H_
#define SRC_ALGORITHMS_FACIALLANDMARK_FACIALLANDMARKIMPL_H_
#include <string>
#include <vector>
#include "facialLandmark.h"
#include "src/Base/arcternalgorithm.h"

namespace arctern {
class FacialLandmarkImpl : public ArcternAlgorithm {
 public:
  FacialLandmarkImpl();

  ~FacialLandmarkImpl() noexcept override;

  ErrorType Init(const InitParameter *p) override;

  ErrorType Process(const RunParameter *p,Result *r) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;

  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

 protected:
  int num_landmark_;
  std::vector<cv::Rect> faceRects_;  ///< adjusted rect
  bool isNormaliized_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACIALLANDMARK_FACIALLANDMARKIMPL_H_
