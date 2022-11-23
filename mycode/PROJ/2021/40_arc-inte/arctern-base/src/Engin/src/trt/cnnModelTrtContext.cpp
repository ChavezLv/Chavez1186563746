//
// Created by chengaohua on 2021/7/8.
//

#include <numeric>
#include "cnnModelTrtContext.h"
#include <numeric>
namespace arctern {
///************************ the implement of CnnModelTRTContext ************************
CnnModelTRTContext::CnnModelTRTContext(int gpu_id, int batchSize):maxBatchSize_(batchSize) {
  assert(gpu_id > -1);
  cudaSetDevice(gpu_id);
  // cv::cuda::setDevice(gpu_id);
  stream = new cudaStream_t();
  cudaStreamCreate(stream);
#if 0 /// flagPriority has been deprecated
  /// set priority for stream
  if(flagPriority){
      int leastPriority, greatestPriority;
      cudaDeviceGetStreamPriorityRange(&leastPriority, &greatestPriority);
      cudaStreamCreateWithPriority(stream, cudaStreamNonBlocking, greatestPriority);
  }
#endif
}

CnnModelTRTContext::~CnnModelTRTContext() {

  if(!bindings_.empty() && bindings_[0] != nullptr){
    auto err = cudaFree(bindings_[0]);
    assert(err == cudaSuccess);
    for(auto & item:bindings_){
      item = nullptr;
    }
    bindings_.clear();
  }

  if (stream) {
    cudaStreamDestroy(*stream); /// free stream
    delete stream;
    stream = nullptr;
  }

  if(context_){
    context_->destroy();
    context_ = nullptr;
  }
}

bool CnnModelTRTContext::init(CnnModelTRTEngine *trtEngine) {
  assert(trtEngine != nullptr);

  context_ = trtEngine->getTrtEngineInstance()->createExecutionContext();

  if (context_ == nullptr) {
    return false;
  }

  nbBindings_ = trtEngine->getNbBindings();

  nbDims_.clear();
  for(int i = 0; i < nbBindings_; i++) {
    nbDims_.push_back(trtEngine->getDimPerBinding(i));
    assert(nbDims_[i].size());
    m_bDynamic =  nbDims_[i][0] < 1 ;
    if(m_bDynamic){
      if(i==0){
        Dims dims;
        dims.nbDims = nbDims_[i].size();
        int j=0;
        std::for_each(nbDims_[i].begin(),nbDims_[i].end(),[&](int dim){dims.d[j++] = dim;});
        dims.d[0] = maxBatchSize_;
        context_->setBindingDimensions(i,dims);
        if(!context_->allInputDimensionsSpecified()){
          std::cout <<"some input not specified dims"<<std::endl;
          return false;
        }
      }
      ///>
      nbDims_[i][0] = 1;
    }

  }

  /// assert the index 0 of bindings is input index
  assert(trtEngine->isBindingInputIndex(0));
  bindings_.resize(nbBindings_, nullptr);

  /// alloc the device memory for bindings_
  alloc_bindings(maxBatchSize_);

  /// init context of trt


  return true;
}

void CnnModelTRTContext::destory() {
  context_->destroy();
  context_ = nullptr;
}

void *CnnModelTRTContext::getPtrOfTrtInputLayer() {
  /// Default, we make the index 0 of bindings as the input layer pointer
  if (bindings_[0]) {
    return bindings_[0];
  }

  return nullptr;
}

void CnnModelTRTContext::getSizePerBindings(std::vector<size_t> &perSize, int batchSize) {
  for (int i = 0; i < nbBindings_; ++i) {
    auto & dims = nbDims_[i];
    size_t size = accumulate(dims.begin(), dims.end(), batchSize, multiplies<size_t>());
    //size_t size = engine_->getSizePerBinding(i, batchSize);
    perSize[i] = size;
  }
}

size_t CnnModelTRTContext::getSizeBindingByIndex(int index, int batchSize) {
  auto & dims = nbDims_[index];
  size_t size = accumulate(dims.begin(), dims.end(), batchSize, multiplies<size_t>());
  return size;
}

void CnnModelTRTContext::alloc_bindings(int batchSize) {
  /// temperary store the size of per bindings
  std::vector<size_t> perSize_t(nbBindings_);
  getSizePerBindings(perSize_t, batchSize);

  size_t sumAllocSize_t = 0;
  for (int i = 0; i < nbBindings_; ++i) {
    sumAllocSize_t += perSize_t[i];
  }

  /// allocate device memory
  void *devMemPtr = nullptr;
  auto err = cudaMalloc(&devMemPtr, sumAllocSize_t * sizeof(float));
  assert(err == cudaSuccess);
  /// alloc the device memory for bindings
  if (devMemPtr) {
    for (int i = 0; i < nbBindings_; ++i) {
      bindings_[i] = devMemPtr;
      devMemPtr = (void *) ((char *) devMemPtr + perSize_t[i] * sizeof(float));
    }
  }

}

int CnnModelTRTContext::forward(std::vector<std::vector<float>> &outputs){
  outputs.resize(nbBindings_ - 1);
  std::vector<size_t> perSize_t(nbBindings_);
  getSizePerBindings(perSize_t, currentBatchSize_);

  /// the index of outputs start from 1 to (nbBindings_ - 1) in bindings
  for (int i = 1; i < nbBindings_; ++i) {
    outputs[i - 1].resize(perSize_t[i]); /// alloc the host memory of outputs
  }

  return forwardImpl(bindings_.data(), outputs, currentBatchSize_);
}

std::vector<std::vector<float>> CnnModelTRTContext::forward() {
  /// the outputs of forward ( Default, the input numbers of bindings is 1 in net)
  std::vector<std::vector<float>> outputs(nbBindings_ - 1);

  std::vector<size_t> perSize_t(nbBindings_);
  getSizePerBindings(perSize_t, currentBatchSize_);

  /// the index of outputs start from 1 to (nbBindings_ - 1) in bindings
  for (int i = 1; i < nbBindings_; ++i) {
    outputs[i - 1].resize(perSize_t[i]); /// alloc the host memory of outputs
  }

  if (forwardImpl(bindings_.data(), outputs, currentBatchSize_) == 0) {
    return outputs;
  } else {
    return std::vector < std::vector < float >> ();
  }
}

int CnnModelTRTContext::forward(std::pair<float *, size_t> *outputs, int outputSize) {
  return forwardImpl(bindings_.data(), outputs, outputSize, currentBatchSize_);
}

int CnnModelTRTContext::forwardImpl(void **bindings, std::vector<std::vector<float>> &outputs, int batchSize) {
  if(m_bDynamic){
    //context_->executeV2(bindings);
    context_->enqueueV2(bindings, *stream, nullptr);
  } else{
    //context_->execute(batchSize,bindings);
    context_->enqueue(batchSize, bindings, *stream, nullptr);
  }
  for (int i = 0; i < outputs.size(); ++i) {
    //TODO: add cuda error print
    /// we defaut bindings[0] as input, so the output index of bindings should plus one
    //auto err = cudaMemcpy(outputs[i].data(), bindings[i + 1],outputs[i].size() * sizeof(float), cudaMemcpyDeviceToHost);
    auto err = cudaMemcpyAsync(outputs[i].data(), bindings[i + 1],outputs[i].size() * sizeof(float),cudaMemcpyDeviceToHost, *stream);

    if(err != cudaSuccess){
      std::cout <<"===***cudaMemcpy***====:" <<(int) err << endl;
    }

    assert(err == cudaSuccess);
  }

  cudaStreamSynchronize(*stream);

  return 0;
}

int CnnModelTRTContext::forwardImpl(void **bindings, std::pair<float *, size_t> *outputs, int outputSize,
                                    int batchSize) {
  context_->enqueue(batchSize, bindings, *stream, nullptr);

  for (int i = 0; i < outputSize; ++i) {
    //TODO: add cuda error print
    /// we defaut bindings[0] as input, so the output index of bindings should plus one
    auto err =cudaMemcpyAsync(outputs[i].first, bindings[i + 1],
                    outputs[i].second * sizeof(float), cudaMemcpyDeviceToHost, *stream);
    assert(err == cudaSuccess);
  }

  return 0;
}

}// end arctern