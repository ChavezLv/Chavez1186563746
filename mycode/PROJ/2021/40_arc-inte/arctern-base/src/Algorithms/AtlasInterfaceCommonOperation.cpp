//
// Created by gh on 2020/12/8.
//

#include "src/Base/arcternalgorithm.h"
#include "AtlasInterfaceCommonOperation.h"
#include "src/Engin/src/atlas/AtlasEngineManager.h"
#include "src/common/algUtil.h"
#include "log/logger.h"

using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternAlgorithm;
using arctern::AtlasInterfaceCommonOperation;

AtlasInterfaceCommonOperation::~AtlasInterfaceCommonOperation() {
}

static int checkAtlasInit(arctern::InitParameter *p) {
  if (nullptr == p || p->model_path.empty()) {
    return -1;
  }

  std::string modelPath = p->model_path;
  auto name = arctern::getPureModelName(modelPath);

  int supportBatch = 0;
  if (name.find("-b1-") != name.npos) {
    supportBatch = 1;
  } else if (name.find("-b8-") != name.npos) {
    supportBatch = 8;
  }

  if (supportBatch != p->batchNum_) {
    spdlog::error("batchNum doesn't match the model name");
    return -1;
  }

  return 0;
}

ErrorType AtlasInterfaceCommonOperation::Init(ArcternAlgorithmEx *impl,const InitParameter *param) {
  auto p = const_cast<InitParameter*>(param);
  auto code = checkAtlasInit(p);
  if (code < 0) {
    return ErrorType::ERR_INVALID_MODEL;
  }

  arctern::AtlasEngineManager engineManager;

  code = engineManager.parseTrtModels(p->model_path);
  if (code != 0) {
    return ErrorType::ERR_INVALID_MODEL;
  }

  ///> get model info
  impl->GetModelsConfigInfo(const_cast<TrtModels *>(engineManager.trt_models_));

  std::vector<std::shared_ptr<AtlasEngine>> engins;
  ErrorType ret = engineManager.CreateEngin(p, engins);
  if (ret != ErrorType::ERR_SUCCESS) {
    return ret;
  }
  // warnning SetEngin must be before Init!!!
  impl->SetEngines(engins);
  // warnning getConfigInfo must be before Init!!!

  ret = impl->Init(p);

  return ret;
}
