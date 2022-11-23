/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_SRC_ENGIN_SRC_RKNN_RKNNUTIL_H_
#define ARCTERN_SRC_ENGIN_SRC_RKNN_RKNNUTIL_H_

#include <vector>
#include "rknn_api.h"
#include "src/common/tensor.h"

namespace arctern {

class RknnModelInfo {
 public:
    RknnModelInfo();
    ~RknnModelInfo() noexcept;

 public:
    bool isValid_ = false;
    uint32_t inputNum = 0;   // the number of input.
    uint32_t outputNum = 0;  // the number of output.
    rknn_sdk_version version;
    std::vector<rknn_tensor_attr> inputAttrs ;
    std::vector<rknn_tensor_attr> outputAttrs;
    std::vector<arctern::TensorShape> outputShapes_;


};

}  // namespace arctern

#endif //ARCTERN_SRC_ENGIN_SRC_RKNN_RKNNUTIL_H_
