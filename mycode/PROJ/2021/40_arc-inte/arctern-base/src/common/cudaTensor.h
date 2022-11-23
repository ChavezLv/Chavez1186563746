/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2021.06.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_SRC_COMMON_CUDATENSOR_H_
#define ARCTERN_BASE_SRC_COMMON_CUDATENSOR_H_

#include "tensorShape.h"
namespace arctern {

template<typename T>
class CudaTensor{
 public:
  CudaTensor(const TensorShape & shape, T * buf, int maxLength): shape_(shape), buf_(buf), maxLen_(maxLength) {

  }

  const TensorShape & shape() const {
    return shape_;
  }

  int setTensor(float ** buf, int bufLen) {

    //todo
   // cudaMemcpy(buf)
   return 0;
  }

 public:
  TensorShape  shape_;
  T * buf_;       ///<  trt engine buffer
  int maxLen_;    ///<  trt engine buffer size
};
}
#endif  // ARCTERN_BASE_SRC_COMMON_CUDATENSOR_H_
