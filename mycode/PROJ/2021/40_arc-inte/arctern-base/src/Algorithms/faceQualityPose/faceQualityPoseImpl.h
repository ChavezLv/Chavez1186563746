/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEQUALITYPOSE_FACEQUALITYPOSEIMPL_H_
#define SRC_ALGORITHMS_FACEQUALITYPOSE_FACEQUALITYPOSEIMPL_H_

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "faceQualityPose.h"

namespace arctern {
class FaceQualityPoseImpl : public ArcternAlgorithm {
 public:
  FaceQualityPoseImpl();

  ~FaceQualityPoseImpl() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  cv::Rect getFace(int width, int height, const cv::Rect &rect);

};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEQUALITYPOSE_FACEQUALITYPOSEIMPL_H_
