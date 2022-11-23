//
// Created by zy on 8/27/20.
//

#include "atlasAclInfer.h"
#include <unistd.h>
#include <sys/time.h>
using namespace arctern;

static ReadWriteLock g_modelLock;

AtlasAclModelManager *AtlasAclModelManager::m_aclModelManager = new arctern::AtlasAclModelManager;

AtlasAclModelManager * AtlasAclModelManager::getInstance(){
  return m_aclModelManager;
}

AtlasAclModelManager::AtlasAclModelManager(){

}

AtlasAclModelManager::~AtlasAclModelManager(){
  for(auto item = m_map.begin();item != m_map.end();item++){
    if((*item).second != nullptr){
      (*item).second->destroy();
    }
  }

  /// destory context
   for(auto item = m_map_context.begin();item != m_map_context.end();item++){
    if((*item).second != nullptr){
      auto ret = aclrtDestroyContext((*item).second);
      if (ret != APP_ERR_OK) {
        LOGASDError << "Failed to destroy context, ret = " << ret << ".";
      }
      (*item).second = nullptr;
    }
  }
  
}

bool AtlasAclModelManager::init(const FileBuffer &file_buffer,const std::string &key,int deviceId){

  WriteLock_Guard guard(&g_modelLock);
  if(m_map.find(key) != m_map.end()) return true;
  /// create acl runtime context
  if(m_map_context.find(key) == m_map_context.end()){
    aclrtContext tmpContext= nullptr;
    auto err = aclrtCreateContext(&tmpContext, deviceId);
    if (err != APP_ERR_OK) {
      LOGASDError << "Failed to create acl context, ret = " << err << ".";
    return false;
    }
    m_map_context[key] = tmpContext;
  }
  

  auto aclModel = new AtlasAclModel(m_map_context[key]);
  auto ret =aclModel->init(file_buffer);
  if(!ret){
      return false;
  }
  m_map[key] = aclModel;

  return true;
}

AtlasAclModel* AtlasAclModelManager::getAclModel(std::string key){
  WriteLock_Guard guard(&g_modelLock);
  return m_map[key];
}

const aclrtContext & AtlasAclModelManager::getAclContext(std::string key){
  WriteLock_Guard guard(&g_modelLock);
  return m_map_context[key];
}

///=======================================================================================================



AtlasAclModel::AtlasAclModel(aclrtContext context):context_(context){}

AtlasAclModel::~AtlasAclModel(){
  AtlasAclModel::destroy();
}

bool AtlasAclModel::init(const FileBuffer &file_buffer){

  auto models = arctern::GetTrtModels(file_buffer.buffer_)->models();
  auto net = models->Get(0);
  auto net_params = net->params(); 

  mode_size_ = net_params->size(); /// get model size
  //pdata_ = (void*)(net_params->data());/// get model data
  pdata_ =(void*) new char[mode_size_];
  memcpy(pdata_,(void*)(net_params->data()),mode_size_);
  /// set acl context
  auto ret = aclrtSetCurrentContext(context_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
    return false;
  }

  /// query size for net and weight of model
  ret = aclmdlQuerySizeFromMem(pdata_, mode_size_, &modelDevPtrSize_, &weightDevPtrSize_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "aclmdlQuerySizeFromMem failed, ret[" << ret << "].";
    return false;
  }

  ret = aclrtMalloc(&weightDevPtr_, weightDevPtrSize_, ACL_MEM_MALLOC_HUGE_FIRST);
  if (ret != APP_ERR_OK) {
    LOGASDError << "aclrtMalloc weightDevPtr failed, ret[" << ret << "].";
    return false;
  }

  return true;
}

void AtlasAclModel::destroy() {
  /// set acl context
  auto ret = aclrtSetCurrentContext(context_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
  }

  /// free device memory
  if(weightDevPtr_){
    ret = aclrtFree(weightDevPtr_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "aclrtFree failed, ret[" << ret << "].";
    }
    weightDevPtr_ = nullptr;
  }
  if(pdata_){
    delete (char*) pdata_;
    pdata_ = nullptr;
  }
}



///=======================================================================================================



AtlasAclInfer::AtlasAclInfer(aclrtContext context):context_(context){
}

AtlasAclInfer::~AtlasAclInfer() {
  
}

bool AtlasAclInfer::init(uint32_t modelId){
  modelId_ = modelId;

  /// set acl context
  auto ret = aclrtSetCurrentContext(context_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
    return false;
  }

  /// model desc handle for getting information of model
  aclmdlDesc *modelDesc = aclmdlCreateDesc();
  if (modelDesc == nullptr) {
    LOGASDError << "aclmdlCreateDesc failed.";
    return false;
  }

  ret = aclmdlGetDesc(modelDesc, modelId_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "aclmdlGetDesc ret fail, ret:" << ret << ".";
    return false;
  }

  modelDesc_.reset(modelDesc, aclmdlDestroyDesc);

  return true;
}

aclmdlDesc * AtlasAclInfer::GetModelDesc() {
  return modelDesc_.get();
}

aclmdlDataset * AtlasAclInfer::CreateAndFillDataset(std::vector<void *> &bufs, std::vector <size_t> &sizes) {
  APP_ERROR ret = APP_ERR_OK;

  aclmdlDataset *dataset = aclmdlCreateDataset();
  if (dataset == nullptr) {
    LOGASDError << "ACL_ModelInputCreate failed.";
    return nullptr;
  }

  for (size_t i = 0; i < bufs.size(); ++i) {
    aclDataBuffer *data = aclCreateDataBuffer(bufs[i], sizes[i]);
    if (data == nullptr) {
      DestroyDataset(dataset);
      LOGASDError << "aclCreateDataBuffer failed.";
      return nullptr;
    }
    
    ret = aclmdlAddDatasetBuffer(dataset, data);
    if (ret != APP_ERR_OK) {
      aclDestroyDataBuffer(data);
      data = nullptr;
      DestroyDataset(dataset);
      LOGASDError << "ACL_ModelInputDataAdd failed, ret[" << ret << "].";
      return nullptr;
    }
  }
  return dataset;
}

void AtlasAclInfer::DestroyDataset(aclmdlDataset *dataset) {
  /// Just release the DataBuffer object and DataSet object, remain the buffer, because it is managerd by user
  if (dataset != nullptr) {
    for (size_t i = 0; i < aclmdlGetDatasetNumBuffers(dataset); i++) {
      aclDataBuffer* dataBuffer = aclmdlGetDatasetBuffer(dataset, i);
      if (dataBuffer != nullptr) {
        aclDestroyDataBuffer(dataBuffer);
        dataBuffer = nullptr;
      }
    }
    aclmdlDestroyDataset(dataset);
    dataset = nullptr;
  }
}



int AtlasAclInfer::infer(std::vector<void *> &inputBufs, std::vector <size_t> &inputSizes,
                         std::vector<void *> &ouputBufs, std::vector <size_t> &outputSizes, 
                         aclrtStream &stream,
                         size_t dynamicBatchSize) {
  /// set acl context
  auto ret = aclrtSetCurrentContext(context_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
    return false;
  }

  /// configure input dataset
  aclmdlDataset *input = nullptr;
  input = CreateAndFillDataset(inputBufs, inputSizes);
  if (input == nullptr) {
    return APP_ERR_COMM_FAILURE;
  }

  /// set dynamic batch size
  if (dynamicBatchSize != 0) {
    size_t index;
    ret = aclmdlGetInputIndexByName(modelDesc_.get(), ACL_DYNAMIC_TENSOR_NAME, &index);
    if (ret != ACL_ERROR_NONE) {
      LOGASDError << "aclmdlGetInputIndexByName failed, maybe static model";
      return APP_ERR_COMM_CONNECTION_FAILURE;
    }

    ret = aclmdlSetDynamicBatchSize(modelId_, input, index, dynamicBatchSize);
    if (ret != ACL_ERROR_NONE) {
      LOGASDError << "dynamic batch set failed, modelId_=" << modelId_ << ", input=" << input << ", index=" << index
               << ", dynamicBatchSize=" << dynamicBatchSize;
      return APP_ERR_COMM_CONNECTION_FAILURE;
    }
  }

  /// configure output data set
  aclmdlDataset *output = nullptr;
  output = CreateAndFillDataset(ouputBufs, outputSizes);
  if (output == nullptr) {
    DestroyDataset(input);
    input = nullptr;
    return APP_ERR_COMM_FAILURE;
  }

  /// inference async
  ret = aclmdlExecuteAsync(modelId_, input, output, stream);
  if (ret != APP_ERR_OK) {
    LOGASDError << "aclmdlExecute failed, ret[" << ret << "].";
    return ret;
  }
  /// wait stream handle finished
  ret = aclrtSynchronizeStream(stream);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to synchronize the stream, ret = " << ret << ".";
    return ret;
  }

  DestroyDataset(input);
  DestroyDataset(output);
  return APP_ERR_OK;
}


