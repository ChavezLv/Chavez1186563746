/****************************************************************************
 *  Filename:       tensorflowengin.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.23
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_TENSORFLOW_TENSORFLOWENGIN_H_
#define SRC_ENGIN_SRC_TENSORFLOW_TENSORFLOWENGIN_H_
#include <vector>
#include "src/Engin/src/enginBase.h"


namespace arctern {
class TensorflowResult : public Result {
    void Release() override {};
    void resize(int n) override {};
};

class TensorflowEngin : public EnginBase {
 public:
    TensorflowEngin();
    ~TensorflowEngin() override;

    ErrorType Init(const InitParameter *p) override;
    void Release() override;
    ErrorType Process(const RunParameter *p,Result *r) override;

    const StatusParameter * GetParameter(StatusParameter *p) override;
    int forward(const Tensor<float> &input_tensor,
          std::vector<arctern::Tensor<float>>& outputTensors) override {return 0;}

 protected:
     TensorflowResult result_;
};
}  // namespace arctern
#endif  // SRC_ENGIN_SRC_TENSORFLOW_TENSORFLOWENGIN_H_
