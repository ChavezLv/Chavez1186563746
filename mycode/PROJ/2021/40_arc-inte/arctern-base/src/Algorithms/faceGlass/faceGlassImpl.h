/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEGLASS_FACEGLASSIMPL_H_
#define SRC_ALGORITHMS_FACEGLASS_FACEGLASSIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "faceGlass.h"

namespace arctern {
class FaceGlassImpl : public ArcternAlgorithm {
 public:
  FaceGlassImpl();

  ~FaceGlassImpl() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}
 private:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  std::pair<int, float> calc_mask(const float *data, size_t num_class);

 protected:


  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEGLASS_FACEGLASSIMPL_H_
