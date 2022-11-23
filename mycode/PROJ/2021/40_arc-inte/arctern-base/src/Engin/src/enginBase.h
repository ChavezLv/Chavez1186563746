/****************************************************************************
 *  Filename:       enginBase.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_ENGINBASE_H_
#define SRC_ENGIN_SRC_ENGINBASE_H_
#include <utility>
#include <vector>
#include <string>
#include "src/Base/algorithmbase.h"
#include "include/initparameter.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/tensor.h"
#include "src/common/Tensor5.h"
using arctern::TensorShape;

namespace arctern {

class EnginBase {
 public:
  EnginBase() = default;
  virtual ~EnginBase() =default;

  const int GetOutputNum();
  const std::vector<TensorShape> &GetInputShape();
  std::string GetVersion() { return version_; }


  virtual int forward(const Tensor<float> &input_tensor,
                      std::vector<arctern::Tensor<float> > &outputTensors);
  virtual int forward(const Tensor<uint8> &input_tensor,
                      std::vector<arctern::Tensor<float> > &outputTensors);

  // for tvm
  virtual void SetOutputShape(std::vector<std::vector<int64_t>> output_shape);

  // for onnx && mnn
  virtual void setOutputName(std::vector<std::string> ops);

  void SetVersion(std::string version) { version_ = std::move(version); }

 public:
  int batchNum_ = 1;

 protected:
  std::vector<TensorShape> inputShape_;
  /// < network output layer num
  int numOutput_;
  std::string version_;
};
}  // namespace arctern
#endif  // SRC_ENGIN_SRC_ENGINBASE_H_
