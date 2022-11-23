/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_TVM_TVMENGIN_H_
#define SRC_ENGIN_SRC_TVM_TVMENGIN_H_
#include <vector>
#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/registry.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/builtin_fp16.cc>


#include "src/Engin/src/enginBase.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/tensor.h"
#include "tvmEngineInitParameter.h"

using arctern::TensorShape;

namespace arctern {

class TvmEngin : public EnginBase {
 public:
  TvmEngin();
  ~TvmEngin() override;

  ErrorType Init(const TvmEngineInitParameter *p) ;
  void Release() ;

  void SetOutputShape(std::vector<std::vector<int64_t>> output_shape) override;

  bool loadModel(const MxnetModel *model);
  int forward(const Tensor<float> &input_tensor,
              std::vector<arctern::Tensor<float> > &outputTensors) override;

 protected:

  void get_result(const std::vector<std::vector<int64_t> > &output_shape, int i, float *y,
                  std::vector<Tensor<float>> &outputs);

  std::vector<std::vector<int64_t>> output_shape_;

  int device_type_;
  int device_id_;
  int host_bits_;

  tvm::runtime::Module mod_;
  tvm::runtime::Module modSyslib_;

  const int input_ndim_ = 4;
  const char *input_key[1] = {"data"};
  int64_t input_shape_[4];
  int num_output_;
  DLTensor *x_ = nullptr;
  DLTensor *x_cl_ = nullptr;    /// < used on GPU
  int host_type_ = kDLCPU;
  int dtype_bits_;    /// < used on GPU
  int dtype_lanes_ = 1;
  int dtype_code_ = kDLFloat;
};
}  // namespace arctern
#endif  // SRC_ENGIN_SRC_TVM_TVMENGIN_H_
