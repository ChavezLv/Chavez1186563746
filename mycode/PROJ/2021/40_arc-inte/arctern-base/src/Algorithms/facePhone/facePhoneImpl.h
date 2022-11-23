/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEPHONE_FACEPHONEIMPL_H_
#define SRC_ALGORITHMS_FACEPHONE_FACEPHONEIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "facePhone.h"

namespace arctern {
class FacePhoneImpl : public ArcternAlgorithm {
 public:
  FacePhoneImpl();

  ~FacePhoneImpl() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}
 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  std::pair<int, float> calcPhone(const float *data, size_t num_class);
 protected:
  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEPHONE_FACEPHONEIMPL_H_
