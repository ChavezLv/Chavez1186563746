/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEPOSE_FACEPOSEIMPL_H_
#define SRC_ALGORITHMS_FACEPOSE_FACEPOSEIMPL_H_
#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "facePose.h"

namespace arctern {
class FacePoseImpl : public ArcternAlgorithm {
 public:
  FacePoseImpl();
  ~FacePoseImpl() override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  cv::Rect getFace(int width, int height, const cv::Rect &rect);

  FacePoseLevel calcResult(const float * data, int num);
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEPOSE_FACEPOSEIMPL_H_
