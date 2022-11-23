/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "src/log/logger.h"

#include "src/common/file_buffer.h"
#include "tvmEngineManager.h"
#include "tvmengin.h"
#include "tvmEngineInitParameter.h"


using arctern::ErrorType;
using arctern::TvmEngineManager;
using arctern::AlgorithmBase;
using arctern::EnginBase;
TvmEngineManager::TvmEngineManager() {
  return;
}
ErrorType TvmEngineManager::CreateEngine(InitParameter *p,
                                         std::vector<std::shared_ptr<EnginBase>> &engines) {
  int code = parseMxnetModels(p->model_path);
  if (0 != code) {
    return ErrorType::ERR_MODEL_NO_EXIST;
  }

  try {
    std::vector<std::shared_ptr<EnginBase>> tmpEngines;
    auto models = mxnet_models_->models();

    ErrorType ret = ErrorType::ERR_SUCCESS;
    for (size_t i = 0; i < models->size(); i++) {
      auto engine = std::make_shared<arctern::TvmEngin>();
      auto model = const_cast<MxnetModel *>(models->Get(i));
      engine->SetVersion(mxnet_models_->version()->str());
      arctern::TvmEngineInitParameter tvmp;
      tvmp.mxnetModel_ = model;
      tvmp.batchNum_ = p->batchNum_;
      tvmp.gpuId_ = p->gpuId_;
      tvmp.deviceType_ = p->deviceType_;
      tvmp.typeBits_  = p->typeBits_;
      ret = engine->Init(&tvmp);
      if (ret != ErrorType::ERR_SUCCESS) {
        break;
      }
      tmpEngines.push_back(engine);
    }
    if (ret != ErrorType::ERR_SUCCESS) {
      SPDLOG_ERROR("error!!!! cannot init engin : when init %s!!!!",
                   p->model_path.c_str());
      return ErrorType::ERR_MODEL_NO_EXIST;
    }
    engines.assign(tmpEngines.begin(), tmpEngines.end());
    return ErrorType::ERR_SUCCESS;
  } catch (std::exception e) {
    SPDLOG_ERROR("cannot create tvm engin!!!! {}", e.what());
    return ErrorType::ERR_MODEL_NO_EXIST;
  }
}

