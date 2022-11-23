/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.17
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEQUALITY_FACEQUALITYIMPL_H_
#define SRC_ALGORITHMS_FACEQUALITY_FACEQUALITYIMPL_H_

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "faceQuality.h"

namespace arctern {
class FaceQualityImpl: public ArcternAlgorithm {
 public:
  FaceQualityImpl();

  ~FaceQualityImpl() override;

 // ErrorType Init(const InitParameter *p) override;

//  void Release() override;


//  int forward(const Tensor<float> &input_tensor,
//              std::vector<arctern::Tensor<float> > &outputs) override;
  int GetConfigInfo(arctern::MxnetModel *model) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

 protected:
 // void initPreProcessor();

 // cv::Mat LocalGetFace(const cv::Mat &image, const RunParameter * p, int idx) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                            std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;



  virtual float calcResult(const float * data0, const float * data1);

 protected:

  bool isNormaliized_;
  int outputNum_;
  bool isGrayInput_;
  bool CLS_FIRST_BR_ = false;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEQUALITY_FACEQUALITYIMPL_H_
