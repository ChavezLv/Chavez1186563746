/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNNINTERFACECOMMONOPERATION_H
#define ARCTERN_RKNNINTERFACECOMMONOPERATION_H

#include "include/arctern.h"
#include "include/initparameter.h"

namespace arctern {
class ArcternAlgorithm;
class RknnInterfaceCommonOperation {
 public:
    RknnInterfaceCommonOperation() = delete;
    ~RknnInterfaceCommonOperation();
    static arctern::ErrorType Init(ArcternAlgorithm **impl,const InitParameter *p);
};
}  // namespace arctern

#endif //ARCTERN_RKNNINTERFACECOMMONOPERATION_H
