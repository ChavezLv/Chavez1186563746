//
// Created by chengaohua on 2021/5/22.
//

#include "trtEngin.h"

namespace arctern {
 TrtEngin::TrtEngin(): context_(nullptr) {

 }

 TrtEngin::~TrtEngin() {
   if (context_) {
     delete context_;
     context_ = nullptr;
   }
 }

int TrtEngin::initUtils(TrtModelInfo& info){
  auto engine = TRTModelResource::getInstance()->getEngine(info.key);
  auto layer = engine->getNbBindings() ;
  for(int i = 1; i< layer; i++) {
    auto dims = engine->getDimPerBinding(i);
    int len = dims.size();
    assert(len > 0);
    if(dims[0] < 0){
      assert(len > 1);
      if(len == 2) {
        outputShape_.emplace_back(TensorShape(dims[1]));
      }else if (len == 3) {
        outputShape_.emplace_back(TensorShape(dims[1], dims[2]));
      }else if (len == 4) {
        outputShape_.emplace_back(TensorShape(dims[1], dims[2], dims[3]));
      }else if(len == 5){
        outputShape_.emplace_back(TensorShape(dims[1], dims[2], dims[3], dims[4]));
      }
    }
    else{
      if(len == 1) {
        outputShape_.emplace_back(TensorShape(dims[0]));
      }else if (len == 2) {
        outputShape_.emplace_back(TensorShape(dims[0], dims[1]));
      }else if (len == 3) {
        outputShape_.emplace_back(TensorShape(dims[0], dims[1], dims[2]));
      }else if(len == 4){
        outputShape_.emplace_back(TensorShape(dims[0], dims[1], dims[2], dims[3]));
      }
    }

  }

  if(context_ == nullptr) {
    context_ = new CnnModelTRTContext(info.gpuId,info.batch);
    context_->init(engine.get());
  }
  return 0;
}

int TrtEngin::initMxnet(TrtModelInfo & info){
  TRTModelResource::getInstance()->registerMxnetModel(info);
  initUtils(info);

  return 0;
}

int TrtEngin::initTrt(TrtModelInfo & info) {
  TRTModelResource::getInstance()->registerTrtModel(info);
  initUtils(info);

  return 0;
}

int TrtEngin::initOnnx(TrtModelInfo& info){
  TRTModelResource::getInstance()->registerTrtOnnxModel(info);
  initUtils(info);

  return 0;
}

int TrtEngin::forward(const std::vector<std::pair<const void *, size_t>> &input,
            std::vector<arctern::OutputTensor<float> > &outputTensors)  {
   int currentBatch = input.size();
   int offset = 0;
   uchar * startPtr =static_cast<uchar *>(context_->getPtrOfTrtInputLayer()) ;
   assert(startPtr != nullptr);
   for(int i = 0 ; i < currentBatch; i++) {
     cudaMemcpyAsync(startPtr + offset, input[i].first, input[i].second, cudaMemcpyDeviceToDevice, *context_->getCudaStream());
      offset += input[i].second;
   }
   cudaStreamSynchronize(*context_->getCudaStream());
   context_->currentBatchSize_ = currentBatch;

   std::vector<std::vector<float>> output;
   auto errCode = context_->forward(output);
   if(errCode != 0) return errCode;

   int num = outputShape_.size() ;
   outputTensors.reserve(num);
   for(int i =0; i < num; i++) {
     size_t size = 1;
     for(int j=0;j<outputShape_[i].num_dims();j++){
       size *= outputShape_[i][j];
     }
     OutputShape shape(currentBatch,size);

     OutputTensor<float> output_tensor(shape);
     memcpy(output_tensor.data(), output[i].data(), output[i].size() * sizeof(float));
     outputTensors.emplace_back(output_tensor);
   }
   return errCode;
}

int TrtEngin::getInputTensor(void **input, int *length) {
   int batch = context_->maxBatchSize_;
   auto len = context_->getSizeBindingByIndex(0, batch);
   *input = context_->getPtrOfTrtInputLayer();
   *length = len;
   std::cout<<"getInputTensor len = "<<len<<std::endl;
   return 0;
 }
}
