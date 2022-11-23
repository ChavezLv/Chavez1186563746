/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_SRC_ALGORITHMS_FACEDETECTTINY_RKNNTINYYOLOIMPL_H_
#define ARCTERN_SRC_ALGORITHMS_FACEDETECTTINY_RKNNTINYYOLOIMPL_H_

#include "../faceDetectTinyImpl.h"

namespace arctern {
class RknnFaceDetectTinyImpl : public faceDetectTinyImpl {
 public:
  RknnFaceDetectTinyImpl();
  ~RknnFaceDetectTinyImpl() override;

  ErrorType Init(const InitParameter *p) override;

  /// \brief hisi use this function;
  int forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float> > &outputs) override;

 protected:
  Tensor<float> transpose(const Tensor<float> &input_tensor);
  Tensor<float> process_output_layers(std::vector<Tensor<float>> &outputs);
};
}  // namespace arctern
#endif //ARCTERN_SRC_ALGORITHMS_FACEDETECTTINY_RKNNTINYYOLOIMPL_H_
