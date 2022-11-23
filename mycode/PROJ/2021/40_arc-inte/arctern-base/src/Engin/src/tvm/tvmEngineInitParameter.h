/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_TVMENGINEINITPARAMETER_H_
#define SRC_ENGIN_SRC_TVMENGINEINITPARAMETER_H_
#include "./include/initparameter.h"

class MxnetModel;
namespace arctern {
class TvmEngineInitParameter : public InitParameter {
 public:
  TvmEngineInitParameter() {}
  ~TvmEngineInitParameter()  {}
  MxnetModel *mxnetModel_ = nullptr;
};
}   // namespace arctern
#endif  // SRC_ENGIN_SRC_TVMENGINEINITPARAMETER_H_
