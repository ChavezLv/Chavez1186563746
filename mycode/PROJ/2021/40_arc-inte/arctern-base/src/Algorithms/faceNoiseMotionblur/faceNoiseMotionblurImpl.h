/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACENOISEMOTIONBLUR_FACENOISEMOTIONBLURIMPL_H_
#define SRC_ALGORITHMS_FACENOISEMOTIONBLUR_FACENOISEMOTIONBLURIMPL_H_
#include <utility>
#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "faceNoiseMotionblur.h"

namespace arctern {
class FaceNoiseMotionblurImpl : public ArcternAlgorithm {
 public:
  FaceNoiseMotionblurImpl();

  ~FaceNoiseMotionblurImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
  //void initPreProcessor();

  //cv::Mat LocalGetFace(const cv::Mat &image, const RunParameter *p, int idx) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  virtual std::pair<int, float> calcALResult(const float *data, size_t num_class) ;

 protected:

  bool isNormaliized_;
  int USE_POS_ = 0;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACENOISEMOTIONBLUR_FACENOISEMOTIONBLURIMPL_H_
