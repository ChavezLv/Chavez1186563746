/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.04
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_PERSONQUALITY_PERSONQUALITYIMPL_H_
#define SRC_ALGORITHMS_PERSONQUALITY_PERSONQUALITYIMPL_H_

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "personCompleteness.h"

namespace arctern {
class PersonCompletenessImpl: public ArcternAlgorithm {
 public:
  PersonCompletenessImpl();

  ~PersonCompletenessImpl() override;

  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;};

  void InitPreprocessorParam() override;
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:
  cv::Rect get_roi(
          const int &img_width,
          const int &img_height,
          const cv::Rect &bbox,
          const std::array<float, 4> ext_scale) const;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                            std::vector<arctern::Tensor<float> > &outputs) override;

 protected:

  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_PERSONQUALITY_PERSONQUALITYIMPL_H_
