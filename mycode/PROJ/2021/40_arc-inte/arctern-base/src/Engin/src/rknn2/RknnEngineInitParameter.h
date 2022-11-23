/****************************************************************************
 *  Filename:       RknnEngineManager.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_RKNN2ENGINEINITPARAMETER_H_
#define SRC_ENGIN_SRC_RKNN2ENGINEINITPARAMETER_H_

#include "RknnUtil.h"
#include "./include/initparameter.h"
#include "src/common/file_buffer.h"

namespace arctern {
class RknnEngineInitParameter : public InitParameter {
 public:
  RknnEngineInitParameter() {}
  ~RknnEngineInitParameter()  {}

  MxnetModel *mxnetModel_ = nullptr;
};
}   // namespace arctern
#endif  // SRC_ENGIN_SRC_RKNNENGINEINITPARAMETER_H_
