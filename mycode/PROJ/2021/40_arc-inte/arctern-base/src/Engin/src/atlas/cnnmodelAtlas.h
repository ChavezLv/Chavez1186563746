//
// Created by gh on 2020/12/9.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_ATLAS_CNNMODELATLAS_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_ATLAS_CNNMODELATLAS_H_

#include "atlasAclInfer.h"
using namespace arctern;
class CnnModelAtlas{
 public:
  CnnModelAtlas(aclrtContext context, int maxBatchsize);
  ~CnnModelAtlas();

  void destory();

  bool init(const AtlasAclModel *aclModel);
  
  std::vector<std::vector<float>> forword(const std::vector<std::pair<const void*, size_t> > &inputBufs,
                                          aclrtStream stream, int batchsize = 1);

 private:
  int getDynamicBatchSize(const int &size);
 private:
  int maxBatchsize_;
  aclrtContext context_;

  std::shared_ptr<AtlasAclInfer> atlasAclInfer_;

  ///> output
  void *outPutDeviceMemory_ = nullptr;
  void *outPutHostMemory_ = nullptr;
 
  std::vector<void *> ouputDeviceBufs_;
  std::vector<void *> outputHostBufs_;

  std::vector<size_t> outputSizes_;

  ///> specified input
  void * inputDeviceMemory_ = nullptr;
  size_t inputSize_ = 0; 
  
  /// for dynamic batch infer
  bool supportDynamicBatch_ = false;
  int maxDynamicBatchSize_;
  
  void *modelWorkDevPtr_ = nullptr;
  uint32_t modelId_ = 0; 

};

#endif //ARCTERN_BASE_SRC_ENGIN_SRC_ATLAS_CNNMODELATLAS_H_
