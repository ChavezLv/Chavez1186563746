/****************************************************************************
 *  Filename:       RknnEngineManager.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#include "./src/log/logger.h"
#include "RknnUtil.h"
#include "rknnengin.h"
#include "RknnEngineManager.h"
#include "RknnEngineInitParameter.h"
#include "include/arctern.h"

using namespace arctern;


RknnEngineManager::RknnEngineManager() {
}

ErrorType RknnEngineManager::CreateEngine( InitParameter *p,
                                          std::vector<std::shared_ptr<EnginBase>> &engines) {

  using namespace arctern;
  int code = parseMxnetModels(p->model_path);
  if (0 != code) {
    return ERR_INVALID_MODEL;
  }

  try {
    auto models = mxnet_models_->models();
    ErrorType ret = ErrorType::ERR_SUCCESS;
    for (size_t i = 0; i < models->size(); i++) {
      auto engine = std::make_shared<arctern::RknnEngin>();
      auto model = const_cast<MxnetModel *>(models->Get(i));
      engine->SetVersion(mxnet_models_->version()->str());

      arctern::RknnEngineInitParameter rknnInitParam;
      rknnInitParam.mxnetModel_ = model;
      ret = engine->Init(&rknnInitParam);
      if (ret != ErrorType::ERR_SUCCESS) {
        break;
      }

      engines.push_back(engine);
    }

    if (ret != ErrorType::ERR_SUCCESS) {
      SPDLOG_ERROR("error!!!! cannot init engin : when init %s!!!!",
                   p->model_path.c_str());
      return ERR_INVALID_MODEL;
    }
    return ErrorType::ERR_SUCCESS;

  } catch (std::exception & e) {
    SPDLOG_ERROR("cannot create rknn engin!!!! {}", e.what());
    return ERR_INVALID_MODEL;
  }
}

