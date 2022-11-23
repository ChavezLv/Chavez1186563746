//
// Created by gh on 2021/8/19.
//

#include "mnnEngineManager.h"
#include "mnnEngin.h"

namespace arctern {
int MnnEngineManager::SetHW(int height,int width){
  h_ = height;
  w_ = width;
}
ErrorType MnnEngineManager::CreateEngine(InitParameter *p, std::vector<std::shared_ptr<EnginBase>> &engines) {
  if(parseMxnetModels(p->model_path)!=0){
    return ErrorType::ERR_INVALID_MODEL;
  }

  try {
    auto ret = false;
    auto models = mxnet_models_->models();
    for (size_t i = 0; i < models->size(); i++) {

      auto engine = std::make_shared<arctern::MnnEngin>();
      engine->SetVersion(mxnet_models_->version()->str());

      auto model = const_cast<MxnetModel *>(models->Get(i));
      ret = engine->loadModel(model,p,h_,w_);
      if (!ret) {
        SPDLOG_ERROR("error!!! load mxnet model failed.");
        break;
      }

      engines.push_back(engine);
    }
    if (!ret) {
      SPDLOG_ERROR("error!!! cannot create mxnet engine:{}",p->model_path.c_str());
      return ErrorType::ERR_CREATE_ENGINE;
    }
    return ErrorType::ERR_SUCCESS;

  } catch (std::exception &e) {
    SPDLOG_ERROR("cannot create mxnet engine,throw exception:{}", e.what());
    return ErrorType::ERR_UNEXPECT;
  }
}
}