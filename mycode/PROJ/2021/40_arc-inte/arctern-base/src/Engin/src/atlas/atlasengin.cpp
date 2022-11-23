//
// Created by gh on 2020/12/8.
//

#include <vector>
#include "src/common/arcterndefs_generated.h"
#include "src/Engin/src/atlas/atlasengin.h"
#include "src/Engin/src/atlas/AtlasEngineManager.h"


#include <exception>
#include <sstream>

using arctern::AtlasEngine;
using arctern::ErrorType;
using arctern::Result;
using arctern::AlgorithmBase;
using arctern::AlgorithmBase;

AtlasEngine::AtlasEngine(int gpuid, int batchsize): gpuId_(gpuid), maxAclBatchSize_(batchsize) {
  
}

AtlasEngine::~AtlasEngine() {
  bool ret = destroyAclStreams(*aclContext_,aclStream_);
  if(!ret) LOGASDError << "failed to destory context and streams for acl runtime";
}

bool AtlasEngine::initAclStreams(const int& deviceId, aclrtContext &context, aclrtStream &stream){
  /// switch context
  auto ret = aclrtSetCurrentContext(context);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
    return false;
  }

  /// create acl runtime stream
  ret = aclrtCreateStream(&stream);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to create the acl stream, ret = " << ret << ".";
    return false;
  }
    
  return true;
}

bool AtlasEngine::destroyAclStreams(aclrtContext &context, aclrtStream &stream){
  /// switch context
  auto ret = aclrtSetCurrentContext(context);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
    return false;
  }

  /// destory stream
  ret = aclrtDestroyStream(stream);
  if(ret != APP_ERR_OK) {
    LOGASDError << "Failed to destroy the stream, ret = " << ret << ".";
    return false;
  }
  stream = nullptr;

  

  return true;
}

bool AtlasEngine::init_acl(const FileBuffer &file_buffer,InitParameter *p) {
  
  ///> get acl model manager
  unsigned int pos = p->model_path.find_last_of('/');
  auto key = p->model_path.substr(pos+1);
  auto atlas_acl_model = AtlasAclModelManager::getInstance();
  auto ret = atlas_acl_model->init(file_buffer,key,p->gpuId_);
  if (ret != true) {
    LOGASDError << "failed to init AtlasAclModelManager for loading model";
    return ret;
  }
  auto aclModel = atlas_acl_model->getAclModel(key);
  aclContext_ = const_cast<aclrtContext*>(&atlas_acl_model->getAclContext(key));

  ///> init context and streams for acl runtime
  ret = initAclStreams(gpuId_, *aclContext_, aclStream_);
  if (!ret) {
    LOGASDError << "failed to init context and streams for acl runtime";
    return ret;
  }

  ///> allocate model device memeory
  ///> load model data from host to device
  ///> get decs info
  ///> allocate output and input device memeory accoding to desc info
  
  v_atlas_infer_handle_ = std::make_shared<CnnModelAtlas>(*aclContext_, maxAclBatchSize_);
  ret = v_atlas_infer_handle_->init(aclModel);
  if (ret != true) {
    LOGASDError << "failed to init atlas infer handle";
    return ret;
  }

  return true;
}

int AtlasEngine::forward(const std::vector<std::pair<const void *, size_t>> &input, 
std::vector<std::vector<float>> &outputs,int batchSize){
  outputs = v_atlas_infer_handle_->forword(input,aclStream_,batchSize);
  return 0;
}
