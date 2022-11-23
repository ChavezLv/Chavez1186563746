/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.18
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEOCCLUSION_FACEOCCLUSIONIMPL_H_
#define SRC_ALGORITHMS_FACEOCCLUSION_FACEOCCLUSIONIMPL_H_
#include <utility>
#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "faceOcclusion.h"

namespace arctern {
class FaceOcclusionImpl : public ArcternAlgorithm {
 public:
  FaceOcclusionImpl();

  ~FaceOcclusionImpl() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  std::pair<int, float> calcResult(const float *data, size_t num_class);

 protected:
  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEOCCLUSION_FACEOCCLUSIONIMPL_H_
