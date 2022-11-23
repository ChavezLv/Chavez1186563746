/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACELIVENESSRGB_FACELIVENESSRGBIMPL_H_
#define SRC_ALGORITHMS_FACELIVENESSRGB_FACELIVENESSRGBIMPL_H_

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "faceLivenessRgb.h"

namespace arctern {
class FaceLivenessRgbImpl: public ArcternAlgorithm {
 public:
  FaceLivenessRgbImpl();

  ~FaceLivenessRgbImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;


  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override;

 protected:
//  void initPreProcessor();
//
//  cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter * p, int idx) override;

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                            std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  ArcternRect getFace(int width, int height, const cv::Rect & rect);
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACELIVENESSRGB_FACELIVENESSRGBIMPL_H_
