/****************************************************************************
 *  Filename:       TensorflowEngineManager.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.08.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_TENSORFLOWENGINEMANAGER_H_
#define SRC_ENGIN_SRC_TENSORFLOWENGINEMANAGER_H_

#include "src/Engin/src/enginmanager.h"
namespace arctern {
class TensorflowEngineManager : public EngineManager {
 public:
    TensorflowEngineManager();
    ~TensorflowEngineManager() override {TensorflowEngineManager::Release();}
    ErrorType CreateEngine(InitParameter *p,
                           std::vector<std::shared_ptr<EnginBase>> &engines) override;
};
}  // namespace arctern
#endif  // SRC_ENGIN_SRC_TENSORFLOWENGINEMANAGER_H_
