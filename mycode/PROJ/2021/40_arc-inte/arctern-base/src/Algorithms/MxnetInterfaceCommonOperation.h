/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.08.31
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_MXNETINTERFACECOMMONOPERATION_H_
#define SRC_ALGORITHMS_MXNETINTERFACECOMMONOPERATION_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {
class ArcternAlgorithm;
class MxnetInterfaceCommonOperation {
 public:
  MxnetInterfaceCommonOperation() = delete;
  ~MxnetInterfaceCommonOperation() = default;
  static ErrorType Init(ArcternAlgorithm** impl,const InitParameter *param);
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_MXNETINTERFACECOMMONOPERATION_H_
