//
// Created by gh on 2020/12/9.
//

#include "cnnmodelAtlas.h"
using namespace arctern;
using uchar=unsigned char;
CnnModelAtlas::CnnModelAtlas(aclrtContext context, int maxBatchsize)
    : context_(context), maxBatchsize_(maxBatchsize) {};

CnnModelAtlas::~CnnModelAtlas() {
  CnnModelAtlas::destory();
}

void CnnModelAtlas::destory() {

  /// set acl context
  auto ret = aclrtSetCurrentContext(context_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
  }

  if (outPutDeviceMemory_) {
    ret = aclrtFree(outPutDeviceMemory_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "aclrtFree  failed, ret[" << ret << "].";
    }
    outPutDeviceMemory_ = nullptr;
  }


  if (outPutHostMemory_) {
    ret = aclrtFreeHost(outPutHostMemory_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "aclrtFreeHost  failed, ret[" << ret << "].";
    }
    outPutHostMemory_ = nullptr;
  }

  if (inputDeviceMemory_) {
    ret = aclrtFree(inputDeviceMemory_);
    if (ret != APP_ERR_OK) {
      LOGASDError << "aclrtFree  failed, ret[" << ret << "].";
    }
    inputDeviceMemory_ = nullptr;
  }

  if(modelWorkDevPtr_){
    ret = aclrtFree(modelWorkDevPtr_);
    if(ret != APP_ERR_OK){
      LOGASDError << "aclrtFree failed, ret[" << ret << "].";
    }
    modelWorkDevPtr_ = nullptr;
  }

  /// unload model
  ret = aclmdlUnload(modelId_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "aclmdlUnload  failed, ret["<< ret << "].";
  }
}


bool CnnModelAtlas::init(const AtlasAclModel *aclModel) {
  /// set acl context
  auto ret = aclrtSetCurrentContext(context_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to set ACL context, ret = " << ret;
    return false;
  }
  
  ret = aclrtMalloc(&modelWorkDevPtr_,aclModel->modelDevPtrSize_, ACL_MEM_MALLOC_HUGE_FIRST);
  if(ret != APP_ERR_OK){
    LOGASDError << "aclrtMalloc modelWorkDevPtr failed, ret[" << ret << "].";
    return false;
  }

  /// load model to device
  ret = aclmdlLoadFromMemWithMem(aclModel->pdata_,aclModel->mode_size_, 
                                 &modelId_, 
                                 modelWorkDevPtr_, aclModel->modelDevPtrSize_,
                                 aclModel->weightDevPtr_, aclModel->weightDevPtrSize_);
  if (ret != APP_ERR_OK) {
    LOGASDError << "aclmdlLoadFromMemWithMem failed, ret[" << ret << "].";
    return false;
  }
  ///> model data from host to device and get desc
  atlasAclInfer_ = std::make_shared<AtlasAclInfer>(context_);
  ret = atlasAclInfer_->init(modelId_);

  if (!ret) {
    LOGASDError << "atlasAclInfer_ init failed";
    return false;
  }

  /// Get model description
  aclmdlDesc *modelDesc = atlasAclInfer_->GetModelDesc();
  size_t outputsNb = aclmdlGetNumOutputs(modelDesc);
  
  size_t outputMemorySize = 0;
  for (int i = 0; i < outputsNb; ++i) {
    /// Get output size according to index
    size_t bufferSize = aclmdlGetOutputSizeByIndex(modelDesc, i);
    outputMemorySize += bufferSize;
    outputSizes_.push_back(bufferSize);
  }

  /// allocate device memory for outputs
  ret = aclrtMalloc(&outPutDeviceMemory_, outputMemorySize, ACL_MEM_MALLOC_NORMAL_ONLY);
  if (ret != APP_ERR_OK) {
    LOGASDError << "aclrtMalloc device Memory failed, ret[" << ret << "].";
    return false;
  }

  /// allocate host memory for sysnc device memory buffer
  ret = aclrtMallocHost(&outPutHostMemory_, outputMemorySize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to print the result, malloc host failed, ret = " << ret << ".";
    return false;
  }

  /// configure device memory and host memory for per output
  size_t addrOffset = 0;
  for (int i = 0; i < outputsNb; ++i) {
    ouputDeviceBufs_.push_back((uchar *) outPutDeviceMemory_ + addrOffset);
    outputHostBufs_.push_back((uchar *) outPutHostMemory_ + addrOffset);
    addrOffset += outputSizes_[i];
  }

  /// whether loaded model support dynatic batchsize
  aclmdlBatch batchInfo;
  ret = aclmdlGetDynamicBatch(modelDesc, &batchInfo);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to get dynamic batch info , ret = " << ret << ".";
    return false;
  }

  /// dynamic batchsize configure info
  if (batchInfo.batchCount != 0) {

    supportDynamicBatch_ = true;
    maxBatchsize_ = batchInfo.batch[batchInfo.batchCount - 1]; /// default maxDynamicBatchSize_ = 8;
   
    size_t index;
    ret = aclmdlGetInputIndexByName(modelDesc, ACL_DYNAMIC_TENSOR_NAME, &index);
    if (ret != ACL_ERROR_NONE) {
      LOGASDError << "aclmdlGetInputIndexByName failed, maybe static model";
      return false;
    }

    inputSize_ = aclmdlGetInputSizeByIndex(modelDesc, index);
   
    ret = aclrtMalloc(&inputDeviceMemory_, inputSize_, ACL_MEM_MALLOC_NORMAL_ONLY);
    if (ret != APP_ERR_OK) {
      LOGASDError << "aclrtMalloc dynamicBatchMem failed, ret[" << ret << "].";
      return false;
    }
    
  }

  return true;
}

int CnnModelAtlas::getDynamicBatchSize(const int &size) {
  int retBatchSize = 1;
  if (size < 2) {
    retBatchSize = 1;
  } else if (size < 3) {
    retBatchSize = 2;
  } else if (size < 5) {
    retBatchSize = 4;
  } else if (size < 9) { /// default, we support maxDynamicBatchSize_ = 8 by dynamic-batch model
    retBatchSize = 8;
  }

  return retBatchSize;
}

std::vector<std::vector<float>> CnnModelAtlas::forword(const std::vector<std::pair<const void *, size_t>> &inputBufs,
                                                       aclrtStream stream, int batchsize) {
  std::vector<void *> inputBuffers;
  std::vector<size_t> inputSizes;

  for (auto &e : inputBufs) {
    inputBuffers.push_back(const_cast<void*>(e.first));
    inputSizes.push_back(e.second);
  }

  /// inference by ACL
  /// supportDynamicBatch_ == true, dynamicBatchSize must be unequal to 0
  int dynamicBatchSize;
  if (supportDynamicBatch_) {
    dynamicBatchSize = getDynamicBatchSize(batchsize);
    ///> add specified input
    inputBuffers.push_back(inputDeviceMemory_);
    inputSizes.push_back(inputSize_);
  } else {
    dynamicBatchSize = 0;
  }

  std::vector<std::vector<float>> outputs;
  auto ret = atlasAclInfer_->infer(inputBuffers, inputSizes, ouputDeviceBufs_, outputSizes_, stream, dynamicBatchSize);
  if (ret != APP_ERR_OK) {
    LOGASDError << "Failed to execute atlasAclInfer infer, ret = " << ret << ".";
  }
  assert(ret == APP_ERR_OK);

  /// sync Device to Host for ouputBufs
  std::vector<size_t> realOutputSize(ouputDeviceBufs_.size());
  for (int i = 0; i < ouputDeviceBufs_.size(); ++i) {
    realOutputSize[i] = (outputSizes_[i] / maxBatchsize_) * batchsize;
    ret = aclrtMemcpy(outputHostBufs_[i],
                      realOutputSize[i],
                      ouputDeviceBufs_[i],
                      realOutputSize[i],
                      ACL_MEMCPY_DEVICE_TO_HOST);
    if (ret != APP_ERR_OK) {
      LOGASDError << "Failed to print result, memcpy device to host failed, ret = " << ret << ".";
    }
    assert(ret == APP_ERR_OK);
  }

  /// return result
  for (int i = 0; i < outputHostBufs_.size(); ++i) {
    float *data = (float *) outputHostBufs_[i];
    std::vector<float> tmp;
    for (int j = 0; j < (realOutputSize[i] / sizeof(float)); ++j) {
      tmp.push_back(data[j]);
    }
    outputs.push_back(tmp);
  }

  return outputs;
}
