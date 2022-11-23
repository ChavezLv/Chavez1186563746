/****************************************************************************
 *  Filename:       MxnetEngineManager.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.08.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_MXNETENGINEMANAGER_H_
#define SRC_ENGIN_SRC_MXNETENGINEMANAGER_H_
#include <vector>
#include "src/Engin/src/enginmanager.h"
#include "./include/runparameter.h"
#include "./src/common/tensor.h"
namespace arctern {

class MxnetEngineManager : public EngineManager {
 public:
    MxnetEngineManager();
    ~MxnetEngineManager() override {MxnetEngineManager::Release();}
    ErrorType CreateEngine(InitParameter *p,
                           std::vector<std::shared_ptr<EnginBase>> &engines) override;
};
}  // namespace arctern
#endif  //  SRC_ENGIN_SRC_MXNETENGINEMANAGER_H_
