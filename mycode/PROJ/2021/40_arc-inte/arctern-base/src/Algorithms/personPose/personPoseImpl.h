// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.29
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_PERSONPOSE_PERSONPOSEIMPL_H_
#define SRC_ALGORITHMS_PERSONPOSE_PERSONPOSEIMPL_H_
#include "src/Base/arcternalgorithm.h"
#include <vector>

namespace arctern {
class personPoseImpl : public ArcternAlgorithm {
public:
  personPoseImpl();
  ~personPoseImpl();

  ErrorType Process(const RunParameter *p,Result *r) override;

  void InitPreprocessorParam() override;
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

protected:
  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

private:
  const float m_fBBoxExtSaclTop;
  std::vector<cv::Rect> m_vUpBBoxes;
};
} // namespace arctern
#endif   //   SRC_ALGORITHMS_PERSONPOSE_PERSONPOSEIMPL_H_
