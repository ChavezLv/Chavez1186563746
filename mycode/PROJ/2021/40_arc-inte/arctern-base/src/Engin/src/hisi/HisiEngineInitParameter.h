//
// Created by gh on 2020/10/21.
//

#ifndef ARCTERN_SRC_ENGIN_SRC_HISIENGINEINITPARAMETER_H_
#define ARCTERN_SRC_ENGIN_SRC_HISIENGINEINITPARAMETER_H_

class MxnetModel;
namespace arctern {
class HisiEngineInitParameter : public InitParameter {
 public:
  HisiEngineInitParameter() {}
  ~HisiEngineInitParameter() override {}
  MxnetModel *mxnetModel_ = nullptr;
};
}   // namespace arctern

#endif //ARCTERN_SRC_ENGIN_SRC_HISIENGINEINITPARAMETER_H_
