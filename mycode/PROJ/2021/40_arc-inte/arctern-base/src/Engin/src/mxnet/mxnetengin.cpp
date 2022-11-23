/****************************************************************************
 *  Filename:       mxnetengin.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/

#include <vector>
#include <algorithm>
#include "src/log/logger.h"
#include "src/common/arcterndefs_generated.h"
#include "src/Engin/src/mxnet/mxnetengin.h"

using arctern::MxnetEngin;
using arctern::ErrorType;
using arctern::Result;
using arctern::AlgorithmBase;


MxnetEngin::MxnetEngin() : netHandle_(nullptr) {

}

MxnetEngin::~MxnetEngin() {
  if (nullptr != netHandle_) {
    MXPredFree(netHandle_);
    netHandle_ = nullptr;
  }
}

bool MxnetEngin::loadModel(const MxnetModel *model, int batch) {
  const uint default_input_shape_idxes[2] = {0, 4};

  auto net_json = model->json();
  auto net_params = model->params();

  const auto input_shape_t = reinterpret_cast<const uint*>(model->input_shape()->data());
  auto input_shape = const_cast<uint*>(input_shape_t);

  input_shape[0] = batch;
  auto input_shape_size_ = model->input_shape()->size();
  const auto input_shape_idxes = (const uint *) (model->input_shape_idxes() ? model->input_shape_idxes()->data() : nullptr);
  const auto input_shape_idxes_size = (model->input_shape_idxes() ? model->input_shape_idxes()->size() : 0);

  numOutput_ = model->num_output();
  input_shape_plain_to_piled((const int *) input_shape,
            input_shape_size_, (const int *) input_shape_idxes,
            input_shape_idxes_size, inputShape_);

  const char *input_keys[inputShape_.size()];
  if (inputShape_.size() > 1) {
    for (size_t i = 0; i < inputShape_.size(); ++i) {
      std::ostringstream oss;
      oss << "data" << i;
      input_keys[i] = oss.str().c_str();
    }
  } else {
    input_keys[0] = "data";
  }

  if (netHandle_) {
    MXPredFree(netHandle_);
  }

  int device_type = (gpuId_ == -1) ? 1 : 2;
  int device_id = (gpuId_ == -1) ? 0 : gpuId_;

  MXPredCreate(net_json->c_str(),
               net_params->data(),
               net_params->size(),
               device_type,
               device_id,
               inputShape_.size(),
               (const char **) input_keys,
               (input_shape_idxes ? input_shape_idxes : default_input_shape_idxes),
               input_shape,
               &netHandle_);

  if(netHandle_ == nullptr) {
     const char* err = MXGetLastError();
     SPDLOG_ERROR("error!!! netHandle_ is nullptr, last err:{}!!!", err);
     return false;
  } 

  return true;
}

int MxnetEngin::forward(std::vector<arctern::Tensor<float> > &outputTensors) {
   auto outputNum = GetOutputNum();
   outputTensors.reserve(outputNum);

  MXPredForwardWithTrainFlag(netHandle_);
  for (int i = 0; i < outputNum; ++i) {
    uint *oshape = nullptr;
    uint oshape_len;
    MXPredGetOutputShape(netHandle_, i, &oshape, &oshape_len);

    if (oshape_len == 1) {
      outputTensors.push_back(Tensor<float>(TensorShape(oshape[0])));
    } else if (oshape_len == 2) {
      outputTensors.push_back(Tensor<float>(TensorShape(oshape[0], oshape[1])));
    } else if (oshape_len == 3) {
      outputTensors.push_back(
          Tensor<float>(TensorShape(oshape[0], oshape[1], oshape[2])));

    } else {
      outputTensors.push_back(Tensor<float>(
          TensorShape(oshape[0], oshape[1], oshape[2], oshape[3])));
    }
    size_t size = outputTensors[i].shape().size();
    MXPredGetOutput(netHandle_, i, outputTensors[i].data(), size);
  }
  return 0;
}

int MxnetEngin::forward(const Tensor<float> &input_tensor,
                        std::vector<arctern::Tensor<float> > &outputTensors) {

  auto inputShape = GetInputShape();
  assert(inputShape.size() == 1);
  uint shape[4] = {(uint) inputShape[0][0], (uint) inputShape[0][1],
                   (uint) inputShape[0][2], (uint) inputShape[0][3]};
  size_t data_size = shape[0] * shape[1] * shape[2] * shape[3];

  assert(input_tensor.size() == data_size);
  MXPredSetInput(netHandle_, "data", input_tensor.data(), data_size);
  MXPredForwardWithTrainFlag(netHandle_);

  auto outputNum = GetOutputNum();
  outputTensors.reserve(outputNum);

  for (int i = 0; i < outputNum; ++i) {
    uint *oshape = NULL;
    uint oshape_len;
    MXPredGetOutputShape(netHandle_, i, &oshape, &oshape_len);

    if (oshape_len == 1) {
      outputTensors.push_back(Tensor<float>(TensorShape(oshape[0])));
    } else if (oshape_len == 2) {
      outputTensors.push_back(Tensor<float>(TensorShape(oshape[0], oshape[1])));
    } else if (oshape_len == 3) {
      outputTensors.push_back(
          Tensor<float>(TensorShape(oshape[0], oshape[1], oshape[2])));

    } else {
      outputTensors.push_back(Tensor<float>(
          TensorShape(oshape[0], oshape[1], oshape[2], oshape[3])));
    }
    size_t size = outputTensors[i].shape().size();
    MXPredGetOutput(netHandle_, i, outputTensors[i].data(), size);
  }
  return 0;
}

bool MxnetEngin::input_shape_plain_to_piled(const int *plain_shapes,
        const size_t plain_shapes_num, const int *plain_idxes,
        const size_t plain_idxes_num,
        std::vector<TensorShape> &piled_shapes) {
  if (plain_shapes == nullptr || plain_shapes_num <= 0) {
    return false;
  }
  if (plain_idxes == nullptr || plain_idxes_num <= 1) {
    TensorShape shape(0, 0, 0, 0);
    for (int i = 0; i < std::min<int>(4, plain_shapes_num); ++i) {
      shape[i] = plain_shapes[i];
    }
    piled_shapes.push_back(shape);
    return true;
  } else {
    for (size_t i = 0; i < plain_idxes_num - 1; ++i) {
      TensorShape shape(0, 0, 0, 0);
      size_t plain_shapes_start_idx = plain_idxes[i];

      if (plain_shapes_start_idx >= plain_shapes_num) {
        break;
      }
      size_t plain_shapes_end_idx = plain_idxes[i + 1];
      if (plain_shapes_start_idx >= plain_shapes_end_idx) {
        piled_shapes.clear();
        return false;
      }
      for (int j = 0; j < std::min<int>(4,
            plain_shapes_end_idx - plain_shapes_start_idx); ++j) {
        shape[j] = plain_shapes[plain_shapes_start_idx + j];
      }
      piled_shapes.push_back(shape);
    }
    return true;
  }
}
