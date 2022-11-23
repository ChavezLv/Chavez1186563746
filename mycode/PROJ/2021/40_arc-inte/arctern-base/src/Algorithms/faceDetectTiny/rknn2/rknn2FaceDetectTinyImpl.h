//
// Created by gh on 2021/8/5.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_RKNN2_RKNN2FACEDETECTTINYIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_RKNN2_RKNN2FACEDETECTTINYIMPL_H_

#include "../faceDetectTinyImpl.h"

namespace arctern {
class Rknn2FaceDetectTinyImpl : public faceDetectTinyImpl {
 public:
  Rknn2FaceDetectTinyImpl();
  ~Rknn2FaceDetectTinyImpl() override;

  ErrorType Init(const InitParameter *p) override;

  /// \brief hisi use this function;
  int forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float> > &outputs) override;

 protected:
  Tensor<float> transpose(const Tensor<float> &input_tensor);
  Tensor<float> process_output_layers(std::vector<Tensor<float>> &outputs);
};
}  // namespace arctern


#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEDETECTTINY_RKNN2_RKNN2FACEDETECTTINYIMPL_H_
