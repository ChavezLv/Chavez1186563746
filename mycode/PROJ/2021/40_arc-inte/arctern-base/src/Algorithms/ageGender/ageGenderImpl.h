/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.31
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_AGEGENDER_AGEGENDERIMPL_H_
#define SRC_ALGORITHMS_AGEGENDER_AGEGENDERIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "ageGender.h"

namespace arctern {

class AgeGenderImpl : public ArcternAlgorithm {
 public:
  AgeGenderImpl();
  ~AgeGenderImpl() override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                            std::vector<arctern::Tensor<float> > &outputs) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  ErrorType CheckRunParam(const RunParameter *p) override;


 protected:
  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_AGEGENDER_AGEGENDERIMPL_H_
