/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEHAT_FACEHATIMPL_H_
#define SRC_ALGORITHMS_FACEHAT_FACEHATIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "faceHat.h"

namespace arctern {
class FaceHatImpl : public ArcternAlgorithm {
 public:
  FaceHatImpl();

  ~FaceHatImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;

  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 private:
 // cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter *p, int idx) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  std::pair<int, float> calcHat(const float *data, size_t num_class);

 protected:


  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEHAT_FACEHATIMPL_H_
