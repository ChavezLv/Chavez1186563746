/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_TVMENGINEMANAGER_H_
#define SRC_ENGIN_SRC_TVMENGINEMANAGER_H_
#include <vector>
#include "src/Engin/src/enginmanager.h"
#include "./include/runparameter.h"
#include "./src/common/tensor.h"
namespace arctern {

class TvmEngineManager : public EngineManager {
 public:
  TvmEngineManager();
  ~TvmEngineManager() override { TvmEngineManager::Release(); }
  ErrorType CreateEngine(InitParameter *p,
                         std::vector<std::shared_ptr<EnginBase>> &engines) override;
};
}  // namespace arctern
#endif  //  SRC_ENGIN_SRC_TVMENGINEMANAGER_H_
