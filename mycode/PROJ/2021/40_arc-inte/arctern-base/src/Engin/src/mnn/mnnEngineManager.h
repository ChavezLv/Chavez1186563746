//
// Created by gh on 2021/8/19.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_MNN_MNNENGINEMANAGER_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_MNN_MNNENGINEMANAGER_H_
#include "../enginmanager.h"
namespace arctern {
class MnnEngineManager : public EngineManager {
 public:
  MnnEngineManager() = default;
  ~MnnEngineManager() override = default;
  ErrorType CreateEngine(InitParameter *p, std::vector<std::shared_ptr<EnginBase>> &engines) override;
  int SetHW(int,int);
 private:
  int h_;
  int w_;
};
}
#endif //ARCTERN_BASE_SRC_ENGIN_SRC_MNN_MNNENGINEMANAGER_H_
