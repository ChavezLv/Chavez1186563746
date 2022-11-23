//
// Created by gh on 2021/8/19.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_MNN_MNNENGIN_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_MNN_MNNENGIN_H_

#include "../enginBase.h"
#include "MNN/Interpreter.hpp"
#include "MNN/Tensor.hpp"

namespace arctern {
class MnnEngin : public EnginBase {
 public:
  MnnEngin();
  ~MnnEngin() override;

  bool loadModel(const MxnetModel *model,InitParameter *p,int h,int w);

  int init(const void* model,size_t modelSize,InitParameter *p,int h,int w);

  int release();
   int forward(const Tensor<float> &input_tensor,
                      std::vector<arctern::Tensor<float> > &outputTensors) override;

   void setOutputName(std::vector<std::string> ops) override;


 private:
  std::vector<std::string> output_names_;
  std::unique_ptr<MNN::Interpreter> interpreter_;
  MNN::Session * session_ = nullptr;
  MNN::Tensor * input_tensor_ = nullptr;
};
}

#endif //ARCTERN_BASE_SRC_ENGIN_SRC_MNN_MNNENGIN_H_
