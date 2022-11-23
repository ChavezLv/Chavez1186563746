/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_TVMINTERFACECOMMONOPERATION_H
#define ARCTERN_BASE_TVMINTERFACECOMMONOPERATION_H

#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {
class ArcternAlgorithm;
class TvmInterfaceCommonOperation {
 public:
  TvmInterfaceCommonOperation() = delete;
  ~TvmInterfaceCommonOperation();
  static ErrorType Init(ArcternAlgorithm** impl, const InitParameter *p);
};
}  // namespace arctern

#endif //ARCTERN_BASE_TVMINTERFACECOMMONOPERATION_H
