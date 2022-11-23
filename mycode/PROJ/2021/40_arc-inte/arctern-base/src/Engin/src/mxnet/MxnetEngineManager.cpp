/****************************************************************************
 *  Filename:       MxnetEngineManager.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.08.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include "src/log/logger.h"

#include "src/Engin/src/mxnet/MxnetEngineManager.h"
#include "src/common/file_buffer.h"
#include "src/Engin/src/mxnet/mxnetengin.h"

using arctern::ErrorType;
using arctern::MxnetEngineManager;
using arctern::AlgorithmBase;
using arctern::EnginBase;

arctern::MxnetEngineManager::MxnetEngineManager() {

}

ErrorType
MxnetEngineManager::CreateEngine(InitParameter *p, std::vector<std::shared_ptr<EnginBase>> &engines) {
  if(parseMxnetModels(p->model_path)!=0){
    return ErrorType::ERR_INVALID_MODEL;
  }

  try {
    auto ret = false;
    auto models = mxnet_models_->models();
    for (size_t i = 0; i < models->size(); i++) {

      auto engine = std::make_shared<arctern::MxnetEngin>();
      engine->SetVersion(mxnet_models_->version()->str());

      auto model = const_cast<MxnetModel *>(models->Get(i));
      ret = engine->loadModel(model,p->batchNum_);
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

