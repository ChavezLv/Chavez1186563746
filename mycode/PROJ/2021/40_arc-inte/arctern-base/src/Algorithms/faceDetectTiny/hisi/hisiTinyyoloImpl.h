/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_SRC_ALGORITHMS_FACEDETECTTINY_HISITINYYOLOIMPL_H_
#define ARCTERN_SRC_ALGORITHMS_FACEDETECTTINY_HISITINYYOLOIMPL_H_

#include "src/Algorithms/faceDetectTiny/tinyyoloImpl.h"
namespace arctern {
class HisiYoloV3 : public YoloV3 {
 public:
  HisiYoloV3();
  ~HisiYoloV3() override;

  void initPreProcessor() override;

  ErrorType Init(const InitParameter *p) override;

  Tensor<float> NNIE_custom_layers(const std::vector<Tensor<float>> &outputs);
  Tensor<float> transpose(const Tensor<float> &input_tensor);

  /// \brief hisi use this function;
  int forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float> > &outputs) override;
};
}
#endif //ARCTERN_SRC_ALGORITHMS_FACEDETECTTINY_HISITINYYOLOIMPL_H_
