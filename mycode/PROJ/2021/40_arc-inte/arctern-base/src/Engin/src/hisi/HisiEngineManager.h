//
// Created by gh on 2020/10/21.
//

#ifndef ARCTERN_SRC_ENGIN_SRC_HISIENGINEMANAGER_H_
#define ARCTERN_SRC_ENGIN_SRC_HISIENGINEMANAGER_H_

#include "include/runparameter.h"
#include "src/common/tensor.h"
#include "src/Engin/src/enginmanager.h"
namespace arctern {
class HisiEngineRunParameter : public RunParameter {
 public:
  const Tensor<uint8> *input_tensor = nullptr;
  std::vector<arctern::Tensor<float>> *outputs = nullptr;
};

class HisiEngineManager : public EngineManager {
 public:
  HisiEngineManager();
  ~HisiEngineManager() override { HisiEngineManager::Release(); }
  ErrorType CreateEngine(InitParameter *p,
                         std::vector<std::shared_ptr<EnginBase>> &engines) override;
};
}

#endif //ARCTERN_SRC_ENGIN_SRC_HISIENGINEMANAGER_H_
