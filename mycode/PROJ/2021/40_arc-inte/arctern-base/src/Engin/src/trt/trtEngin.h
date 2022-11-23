//
// Created by chengaohua on 2021/5/22.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTENGIN_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTENGIN_H_

#include "TRTModel.h"
#include "src/common/file_buffer.h"
#include "src/common/cudaTensor.h"
#include "src/common/tensor.h"

namespace arctern {
class TrtEngin  {
 public:
  TrtEngin() ;
  ~TrtEngin();
  int initMxnet(TrtModelInfo & info);

  int initTrt(TrtModelInfo & info);

  int initOnnx(TrtModelInfo& info);

  int forward(const std::vector<std::pair<const void *, size_t>> &input,
              std::vector<arctern::OutputTensor<float> > &outputTensors);
  CnnModelTRTContext *getContext(){
    return context_;
  }

  /*
   * get buf address of engine
   */
  int getInputTensor(void ** input, int * length);

 private:
  int initUtils(TrtModelInfo& info);
  CnnModelTRTContext *  context_;
  std::vector<TensorShape> outputShape_;

};
}

#endif //ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTENGIN_H_
