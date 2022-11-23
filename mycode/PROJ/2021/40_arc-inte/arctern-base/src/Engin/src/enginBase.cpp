/****************************************************************************
 *  Filename:       EnginBase.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/

#include <vector>
#include "src/log/logger.h"
#include "enginBase.h"
using arctern::ErrorType;
using arctern::Result;
using arctern::AlgorithmBase;
using arctern::EnginBase;

const int EnginBase::GetOutputNum() {
  return numOutput_;
}

const std::vector<TensorShape> &EnginBase::GetInputShape() {
  return inputShape_;
}

int EnginBase::forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float> > &outputTensors) {
  return 0;
}

int EnginBase::forward(const Tensor<float> &input_tensor, std::vector<arctern::Tensor<float> > &outputTensors) {
  return 0;
}

void EnginBase::SetOutputShape(std::vector<std::vector<int64_t>> output_shape) {
}

void EnginBase::setOutputName(std::vector<std::string> ops) {

}