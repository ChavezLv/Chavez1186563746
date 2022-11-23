/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNN2INTERFACECOMMONOPERATION_H
#define ARCTERN_RKNN2INTERFACECOMMONOPERATION_H

#include <include/initparameter.h>

namespace arctern {
class ArcternAlgorithm;
class Rknn2InterfaceCommonOperation {
 public:
    Rknn2InterfaceCommonOperation() = delete;
    ~Rknn2InterfaceCommonOperation();
    static arctern::ErrorType Init(ArcternAlgorithm **impl,const InitParameter *p);
};
}  // namespace arctern

#endif //ARCTERN_RKNNINTERFACECOMMONOPERATION_H
