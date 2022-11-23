/****************************************************************************
 *  Filename:       rknnengin.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_RKNN_RKNNENGIN_H_
#define SRC_ENGIN_SRC_RKNN_RKNNENGIN_H_

#include "RknnUtil.h"
#include "RknnEngineInitParameter.h"
#include "../enginBase.h"
#include "./src/common/file_buffer.h"


namespace arctern {

class RknnEngin : public EnginBase {
 public:
    RknnEngin();
    ~RknnEngin() override;

    ErrorType Init(const RknnEngineInitParameter *p) ;
    void Release() ;

    int forward(const Tensor<uint8> &inputTensor,
            std::vector<arctern::Tensor<float>> &outputTensors) override;

 protected:
    int loadRknnModel(MxnetModel *model);
    int getModelInfo();

    void printTensorAttr(rknn_tensor_attr &attr);

 protected:
    std::pair<rknn_context, bool> rknnCtx_;
    RknnModelInfo modelInfo_;

};
}  // namespace arctern
#endif  // SRC_ENGIN_SRC_RKNN_RKNNENGIN_H_
