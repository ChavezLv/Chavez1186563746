//
// Created by gh on 2020/12/8.
//

#include "atlasengin.h"
#include "AtlasEngineManager.h"
#include "log/logger.h"
#include "src/common/aes_cryptor.h"
#include "src/common/release_utils.h"
using arctern::ErrorType;
using arctern::AtlasEngineManager;
using arctern::AlgorithmBase;
using arctern::EnginBase;
AtlasEngineManager::AtlasEngineManager() {
  return;
}

AtlasEngineManager::~AtlasEngineManager() {
      if (nullptr != fileBuffer_) {
        delete fileBuffer_;
        fileBuffer_ = nullptr;
    }
}
ErrorType AtlasEngineManager::CreateEngin(InitParameter *p,
                                          std::vector<std::shared_ptr<AtlasEngine>> &engines) {
  auto engine = std::make_shared<arctern::AtlasEngine>(p->gpuId_, p->batchNum_);
  auto ret = engine->init_acl(*fileBuffer_,p);
  if(!ret){
    std::cout << "init acl ret = " << ret << std::endl;
    return ERR_CREATE_ENGINE;  
  }
  engines.push_back(engine);
  return ErrorType::ERR_SUCCESS;

}

int AtlasEngineManager::parseTrtModels(std::string modelPath) {
  try {


#ifdef PLAIN_MODEL
    fileBuffer_ = new FileBuffer(modelPath);
#else
    fileBuffer_ = new FileBuffer();
    GENERATE_RELEASE_KEYS_BLOCK;
    AESCryptor aes_cryptor(RELEASE_KEY_STRING);
    FileBuffer fbmobel(modelPath);
    aes_cryptor.decrypt_file(fbmobel, *fileBuffer_);

#endif

  } catch (std::bad_alloc &e) {
    SPDLOG_ERROR("error!!!! cannot new filebuffer!!!!");
    return -1;
  }

  if (nullptr == fileBuffer_->GetBuffer()) {
    SPDLOG_ERROR("error!!!! cannot init model,please check model path: {}!!!!",
                 modelPath.c_str());
    trt_models_ = nullptr;
    return -1;
  }
  trt_models_ = GetTrtModels(fileBuffer_->buffer_);
  return 0;
}
