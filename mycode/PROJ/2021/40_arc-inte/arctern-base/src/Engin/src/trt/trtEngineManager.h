//
// Created by chengaohua on 2021/5/22.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTENGINEMANAGER_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTENGINEMANAGER_H_

#include <src/Base/arcternalgorithm.h>
#include "src/Engin/src/enginmanager.h"
#include "trtEngin.h"

namespace arctern {

class TrtEngineManager {
 public:
  explicit TrtEngineManager(EngineManager * manager) : manager_(manager) {

  }
  ~TrtEngineManager() {
  }

  ErrorType CreateEnginWithTrt(TrtModelInfo *pInfo,std::vector<std::shared_ptr<TrtEngin>> &engines);

  ErrorType CreateEnginWithMxnet(TrtModelInfo *pInfo, std::vector<std::shared_ptr<TrtEngin>> &engines);

  ErrorType CreateTrtEngineWithOnnx(TrtModelInfo *pInfo,std::vector<std::shared_ptr<TrtEngin>> &engines);
  EngineManager * manager_;

};
}

#endif //ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTENGINEMANAGER_H_
