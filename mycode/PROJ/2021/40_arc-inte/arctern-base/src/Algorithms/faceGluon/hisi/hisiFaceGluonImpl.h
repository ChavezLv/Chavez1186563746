/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.2
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_HISIFACEGLUONIMPL_H
#define ARCTERN_HISIFACEGLUONIMPL_H

#include "../faceGluonImpl.h"

namespace arctern {
class HisiFaceGluonImpl : public FaceGluonImpl {
 public:
  HisiFaceGluonImpl();
  ~HisiFaceGluonImpl() override;
  ErrorType Init(const InitParameter *p) override;

  cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter *p, int idx) override;

  ErrorType CommonProProcess(const RunParameter *p) override;

  void STNProcessing(Tensor<uint8_t> &output, Tensor<uint8_t> &input, Tensor<float> &theta_);
};
}

#endif //ARCTERN_HISIFACEGLUONIMPL_H
