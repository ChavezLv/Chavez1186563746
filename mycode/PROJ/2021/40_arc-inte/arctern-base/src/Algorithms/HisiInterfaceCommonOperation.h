/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_HISIINTERFACECOMMONOPERATION_H
#define ARCTERN_HISIINTERFACECOMMONOPERATION_H

#include <include/export_errorcode.h>
#include <include/initparameter.h>

namespace arctern {
class ArcternAlgorithm;
class HisiInterfaceCommonOperation {
 public:
  HisiInterfaceCommonOperation() = delete;
  ~HisiInterfaceCommonOperation();
  static arctern::ErrorType Init(ArcternAlgorithm **impl, InitParameter *p);
};
}  // namespace arctern


#endif //ARCTERN_HISIINTERFACECOMMONOPERATION_H
