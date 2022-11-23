/****************************************************************************
 *  Filename:       RknnEngineManager.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_RKNN2_RKNNENGINEMANAGER_H_
#define SRC_ENGIN_SRC_RKNN2_RKNNENGINEMANAGER_H_

#include "RknnUtil.h"
#include "../enginmanager.h"
#include "src/common/tensor.h"
#include "include/runparameter.h"

namespace arctern {

class RknnEngineManager : public EngineManager {
 public:
    RknnEngineManager();
    ~RknnEngineManager() override {
        RknnEngineManager::Release();
    }

    ErrorType CreateEngine(InitParameter *p,
                           std::vector<std::shared_ptr<EnginBase>> &engines) override;
};

}  // namespace arctern
#endif  // SRC_ENGIN_SRC_RKNN_RKNNENGINEMANAGER_H_
