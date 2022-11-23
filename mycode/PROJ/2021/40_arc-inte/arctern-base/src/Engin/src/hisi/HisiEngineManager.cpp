//
// Created by gh on 2020/10/21.
//

#include "HisiEngineManager.h"
#include "../enginmanager.h"
#include "log/logger.h"
#include "HisiEngineInitParameter.h"
#include "hisiengin.h"

using arctern::HisiEngineManager;
using arctern::ErrorType;

HisiEngineManager::HisiEngineManager() {
}

ErrorType HisiEngineManager::CreateEngine(InitParameter *p,
                                          std::vector<std::shared_ptr<EnginBase>> &engines) {
  int code = parseMxnetModels(p->model_path);
  if (0 != code) {
    return ErrorType::ERR_BAD_MODELS;
  }

  std::vector<std::shared_ptr<EnginBase>> tmpEngines;
  ErrorType ret = ErrorType::ERR_SUCCESS;
  try {
    auto models = mxnet_models_->models();

    int modelNum = models->size();

    for (int i = 0; i < modelNum; i++) {
      auto engine = std::make_shared<arctern::HisiEngin>();
      auto model = const_cast<MxnetModel *>(models->Get(i));
      engine->SetVersion(mxnet_models_->version()->str());
      arctern::HisiEngineInitParameter hisip;
      hisip.mxnetModel_ = model;
      hisip.batchNum_ = p->batchNum_;

      ret = engine->Init(&hisip);

      if (ret != ErrorType::ERR_SUCCESS) {
        break;
      }
      tmpEngines.push_back(engine);
    }

    if (ret != ErrorType::ERR_SUCCESS) {
      SPDLOG_ERROR("error!!!! cannot init engin : when init %s!!!!",
                   p->model_path.c_str());
      return ErrorType::ERR_BAD_ENGIN;
    }
    engines.assign(tmpEngines.begin(), tmpEngines.end());
    return ErrorType::ERR_SUCCESS;
  } catch (std::exception e) {
    SPDLOG_ERROR("cannot create mxt engin!!!! {}", e.what());
    return ErrorType::ERR_BAD_ENGIN;
  }
}