//
// Created by chengaohua on 2021/5/22.
//

#include "trtEngineManager.h"
#include "trtEngin.h"

namespace arctern {


ErrorType TrtEngineManager::CreateEnginWithTrt(TrtModelInfo *pInfo,
                                               std::vector<std::shared_ptr<TrtEngin>> &engines) {
  auto engin = std::make_shared<TrtEngin>();

  engin->initTrt(*pInfo);

  engines.emplace_back(engin);

  return ErrorType::ERR_SUCCESS;

}

ErrorType TrtEngineManager::CreateEnginWithMxnet(TrtModelInfo *pInfo,
                                                 std::vector<std::shared_ptr<TrtEngin>> &engines) {
  auto engin = std::make_shared<TrtEngin>();

  engin->initMxnet(*pInfo);

  engines.emplace_back(engin);

  return ErrorType::ERR_SUCCESS;
}

ErrorType TrtEngineManager::CreateTrtEngineWithOnnx(TrtModelInfo *pInfo,
                                                    std::vector<std::shared_ptr<TrtEngin>> &engines){
  auto engine = std::make_shared<TrtEngin>();

  engine->initOnnx(*pInfo);

  engines.emplace_back(engine);

  return ErrorType::ERR_SUCCESS;
}

}