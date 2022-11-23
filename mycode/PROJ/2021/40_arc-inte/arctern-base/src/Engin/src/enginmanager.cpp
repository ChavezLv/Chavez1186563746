/****************************************************************************
 *  Filename:       enginmanager.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include "src/log/logger.h"
#include "enginmanager.h"
#include "src/common/file_buffer.h"
#include "src/common/aes_cryptor.h"
#include "src/common/release_utils.h"


using arctern::EngineManager;

using arctern::ErrorType;
using arctern::Result;
using arctern::AlgorithmBase;
using arctern::EnginBase;

EngineManager::EngineManager() {
}

EngineManager::~EngineManager() {
  EngineManager::Release();
}

void EngineManager::Release() {
  if (nullptr != fileBuffer_) {
    delete fileBuffer_;
    fileBuffer_ = nullptr;
  }
}

int EngineManager::parseMxnetModels(std::string modelPath) {
  try {

#ifdef PLAIN_MODEL
    fileBuffer_ = new FileBuffer(modelPath);
#else
    fileBuffer_ = new FileBuffer();
    GENERATE_RELEASE_KEYS_BLOCK;
    AESCryptor aes_cryptor(RELEASE_KEY_STRING);
    FileBuffer fbmobel(modelPath);
    aes_cryptor.decrypt_file(fbmobel, *fileBuffer_);
    SPDLOG_INFO("decrypt model success.");
#endif

  } catch (std::bad_alloc &e) {
    SPDLOG_ERROR("error!!! cannot new file buffer!!!!");
    return -1;
  }

  if (nullptr == fileBuffer_->GetBuffer()) {
    SPDLOG_ERROR("error!!! cannot init model,please check model path:{}!!!",modelPath.c_str());
    mxnet_models_ = nullptr;
    return -1;
  }
  mxnet_models_ = GetMxnetModels(fileBuffer_->buffer_);
  return 0;
}

ErrorType EngineManager::CreateEngine(InitParameter *p, std::vector<std::shared_ptr<EnginBase>> &engines) {
  return ErrorType::ERR_SUCCESS;
}