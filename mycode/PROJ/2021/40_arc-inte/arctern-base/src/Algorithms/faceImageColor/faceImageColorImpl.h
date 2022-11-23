/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACEIMAGECOLOR_FACEIMAGECOLORIMPL_H_
#define SRC_ALGORITHMS_FACEIMAGECOLOR_FACEIMAGECOLORIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "faceImageColor.h"

namespace arctern {
class FaceImageColorImpl : public ArcternAlgorithm {
 public:
  FaceImageColorImpl();

  ~FaceImageColorImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override;
 protected:
 // cv::Mat LocalGetFace(const cv::Mat &image, const RunParameter *p, int idx) override;
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  bool isNormaliized_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEIMAGECOLOR_FACEIMAGECOLORIMPL_H_
