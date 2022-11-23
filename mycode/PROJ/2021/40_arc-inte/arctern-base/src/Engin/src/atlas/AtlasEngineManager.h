//
// Created by gh on 2020/12/8.
//

#ifndef ARCTERN_BASE_ATLASENGINEMANAGER_H
#define ARCTERN_BASE_ATLASENGINEMANAGER_H

#include <vector>
#include "src/Engin/src/enginmanager.h"
#include "include/runparameter.h"
#include "src/common/tensor.h"
namespace arctern {

class AtlasEngineManager {
 public:
  AtlasEngineManager();
  ~AtlasEngineManager();

  int parseTrtModels(std::string modelPath);
  ErrorType CreateEngin(InitParameter *p,
                        std::vector<std::shared_ptr<AtlasEngine>> &engines);
 public:
  const arctern::TrtModels *trt_models_ = nullptr;
  FileBuffer * fileBuffer_ = nullptr;
};
}  // namespace arctern

#endif //ARCTERN_BASE_ATLASENGINEMANAGER_H
