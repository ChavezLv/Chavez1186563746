/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <vector>
#include <algorithm>
#include "src/common/arcterndefs_generated.h"
#include "tvmEngineInitParameter.h"
#include "tvmengin.h"



using arctern::ErrorType;
using arctern::Result;

using arctern::AlgorithmBase;
using arctern::AlgorithmBase;
using arctern::TvmEngin;
using arctern::Tensor;
using arctern::TensorShape;

TvmEngin::TvmEngin() : x_(nullptr), x_cl_(nullptr) {
  return;
}

TvmEngin::~TvmEngin() {
  TvmEngin::Release();
}

ErrorType TvmEngin::Init(const TvmEngineInitParameter *p) {
  int gpu_id = p->gpuId_;
  if (p->deviceType_ == arctern::ARCTERN_CPU) {
    device_type_ = kDLCPU;
    device_id_ = 0;
    dtype_bits_ = 32;
    host_bits_ = 32;
  } else {
    auto tmp = p->typeBits_;
    device_type_ = kDLOpenCL;
    device_id_ = gpu_id < 0 ? 0 : gpu_id;
    dtype_bits_ =  tmp == ArcternTypeBits::ARCTERN_FP32 ? 32 : 16;
    host_bits_ = 32;
  }

  auto ret = loadModel(p->mxnetModel_);
  return ret ? ErrorType::ERR_SUCCESS : ErrorType::ERR_MODEL_NO_EXIST;
}

void TvmEngin::Release() {
  if (nullptr != x_) {
    TVMArrayFree(x_);
  }
  if (device_type_ != kDLCPU) {
    if (nullptr != x_cl_) {
      TVMArrayFree(x_cl_);
    }
  }
  // how to release model?????
  return;
}

bool TvmEngin::loadModel(const MxnetModel *model) {
  modSyslib_ = (*tvm::runtime::Registry::Get("runtime.SystemLib"))();

  auto net_json = model->json();
  auto net_params = model->params();

  const int *input_shape = model->input_shape()->data();
  input_shape_[0] = input_shape[0];
  input_shape_[1] = input_shape[1];
  input_shape_[2] = input_shape[2];
  input_shape_[3] = input_shape[3];

  num_output_ = model->num_output();

  mod_ = (*tvm::runtime::Registry::Get("tvm.graph_runtime.create"))(net_json->str(),
                                                                    modSyslib_, device_type_, device_id_);
  TVMByteArray params_arr;
  params_arr.data = (const char *) net_params->data();
  params_arr.size = net_params->size();
  tvm::runtime::PackedFunc load_params = mod_.GetFunction("load_params");
  load_params(params_arr);
  if (device_type_ != kDLCPU) {
    TVMArrayAlloc(input_shape_, input_ndim_, dtype_code_, host_bits_, dtype_lanes_,
                  host_type_, device_id_, &x_);
    TVMArrayAlloc(input_shape_, input_ndim_, dtype_code_, dtype_bits_, dtype_lanes_,
                  device_type_, device_id_, &x_cl_);
    assert(x_ != nullptr);
    assert(x_cl_ != nullptr);
  } else {
    TVMArrayAlloc(input_shape_, input_ndim_, dtype_code_, host_bits_, dtype_lanes_,
                  host_type_, device_id_, &x_);
    assert(x_ != nullptr);
  }
  return true;
}


int TvmEngin::forward(const Tensor<float> &input_tensor, std::vector<arctern::Tensor<float> > &outputTensors) {
  if (num_output_ != output_shape_.size()) {
    std::cerr << "Num of output is mismatch!" << std::endl;
    return -1;
  }

  if (device_type_ != kDLCPU) {
    const float *it_p = input_tensor.data();
    if (16 == dtype_bits_) {
      uint16_t *x_p = static_cast<uint16_t *>(x_->data);
      std::transform(it_p, it_p + input_tensor.size(), x_p, [](float f) -> uint16_t {
        return __gnu_f2h_ieee(f);
      });
      TVMArrayCopyFromBytes(x_cl_, x_->data, input_tensor.size() * sizeof(uint16_t));
    } else {
      TVMArrayCopyFromBytes(x_cl_, (float *) input_tensor.data(), input_tensor.size() * sizeof(uint32_t));
    }
  } else {
    memcpy((char *) (x_->data), (const char *) input_tensor.data(),
           input_tensor.size() * sizeof(float));
  }

  tvm::runtime::PackedFunc set_input = mod_.GetFunction("set_input");
  device_type_ != kDLCPU ? set_input("data", x_cl_) : set_input("data", x_);
  tvm::runtime::PackedFunc run = mod_.GetFunction("run");
  run();

  if (device_type_ != kDLCPU) {
    tvm::runtime::PackedFunc get_output = mod_.GetFunction("get_output");
    for (int i = 0; i < num_output_; ++i) {
      DLTensor *y_g;
      DLTensor *y_c;

      TVMArrayAlloc(output_shape_[i].data(),
                    output_shape_[i].size(),
                    dtype_code_,
                    dtype_bits_,
                    dtype_lanes_,
                    host_type_,
                    device_id_,
                    &y_g);
      TVMArrayAlloc(output_shape_[i].data(),
                    output_shape_[i].size(),
                    dtype_code_,
                    host_bits_,
                    dtype_lanes_,
                    host_type_,
                    device_id_,
                    &y_c);
      get_output(i, y_g);

      float *y_p;
      if (16 == dtype_bits_) {
        uint16_t *y_g_p = static_cast<uint16_t *>(y_g->data);
        y_p = static_cast<float *>(y_c->data);
        std::transform(y_g_p, y_g_p + output_shape_[i][1], y_p, [](uint16_t h) -> float {
          return __gnu_h2f_ieee(h);
        });
      } else {
        y_p = static_cast<float *>(y_g->data);
      }
      get_result(output_shape_, i, y_p, outputTensors);

      TVMArrayFree(y_g);
      TVMArrayFree(y_c);
    }
    // eval the times
    //  eval_time(20, 10, 0);
  } else {
    tvm::runtime::PackedFunc get_output = mod_.GetFunction("get_output");
    for (int i = 0; i < num_output_; ++i) {
      DLTensor *y;
      TVMArrayAlloc(output_shape_[i].data(),
                    output_shape_[i].size(),
                    dtype_code_,
                    host_bits_,
                    dtype_lanes_,
                    host_type_,
                    device_id_,
                    &y);
      get_output(i, y);


      // eval the times
      // eval_time(20, 10, 0);
      auto y_iter = static_cast<float *>(y->data);
      get_result(output_shape_, i, y_iter, outputTensors);

      TVMArrayFree(y);
    }
  }
  return 0;
}


void TvmEngin::get_result(const std::vector<std::vector<int64_t> > &output_shape, int i, float *y,
                          std::vector<Tensor<float>> &outputs) {
  auto size = output_shape[i].size();

  TensorShape shape;
  switch (size) {
    case 1: {
      outputs.push_back(Tensor<float>(TensorShape(output_shape[i][0])));
      break;
    }
    case 2: {
      outputs.push_back(Tensor<float>(TensorShape(output_shape[i][0], output_shape[i][1])));;
      break;
    }
    case 3: {
      outputs.push_back(Tensor<float>(TensorShape(output_shape[i][0], output_shape[i][1], output_shape[i][2])));
      break;
    }
    default: {
      outputs.push_back(Tensor<float>(TensorShape(output_shape[i][0],
                                                  output_shape[i][1], output_shape[i][2], output_shape[i][3])));
    }
  }

  size = outputs[i].shape().size();
  memcpy(outputs[i].data(), y, size * sizeof(float));
}

void TvmEngin::SetOutputShape(std::vector<std::vector<int64_t>> output_shape) {
  output_shape_ = output_shape;
}

