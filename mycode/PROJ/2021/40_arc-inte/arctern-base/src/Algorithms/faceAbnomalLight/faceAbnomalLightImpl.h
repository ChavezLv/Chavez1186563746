/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEABNOMALLIGHT_FACEABNOMALLIGHTIMPL_H_
#define SRC_ALGORITHMS_FACEABNOMALLIGHT_FACEABNOMALLIGHTIMPL_H_
#include <vector>
#include <utility>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "faceAbnomalLight.h"

namespace arctern {
class FaceAbnomalLightImpl: public ArcternAlgorithm {
 public:
  FaceAbnomalLightImpl();
  ~FaceAbnomalLightImpl() override;
  //ErrorType Init(const InitParameter *p) override;
 // void Release() override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:
 // void initPreProcessor();
  //cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter * p, int idx) override;
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                            std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;

  std::pair<int, float> calcALResult(const float *data, size_t num_class);
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEABNOMALLIGHT_FACEABNOMALLIGHTIMPL_H_
