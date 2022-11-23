/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEGLUON_FACEGLUONIMPL_H_
#define SRC_ALGORITHMS_FACEGLUON_FACEGLUONIMPL_H_
#include <vector>
#include <string>
#include "src/Base/arcternalgorithm.h"
#include "faceGluon.h"
#include "src/Algorithms/faceGluon/utils/faceGluonUtil.h"
namespace arctern {
class FaceGluonImpl : public ArcternAlgorithm {
 public:
  FaceGluonImpl();

  ~FaceGluonImpl() override;

  void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;

  ErrorType LocalPreProcess( const arctern::RunParameter *p, int idx);

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:

 protected:
  std::array<float, 2> exthScale;
  std::array<float, 2> extwScale;
  std::string version_;  ///< version num
  bool needLandmark_;   ///< align face
  bool maskWipe_ = false;
  bool isNormaliized_;
  gluon::LocalPreProcessV type_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEGLUON_FACEGLUONIMPL_H_
