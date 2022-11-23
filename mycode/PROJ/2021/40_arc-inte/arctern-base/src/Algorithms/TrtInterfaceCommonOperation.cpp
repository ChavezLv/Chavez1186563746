//
// Created by chengaohua on 2021/5/22.
//

#include "TrtInterfaceCommonOperation.h"
#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Engin/src/trt/trtEngineManager.h"
#include "src/Engin/src/enginmanager.h"
#include "src/common/common.h"
#include <cuda_runtime.h>
namespace arctern {
bool TrtInterfaceCommonOperation::isOnnxModel(const char * const buffer){
  auto models = GetMxnetModels(buffer);
  std::string framework;
  auto model = const_cast<MxnetModel*>(models->models()->Get(0));
  auto bRet = try_get_value(*model,"framework",framework);
  if(bRet && framework == "onnx") return true;
  return false;
}
std::string typeBits2string(arctern::ArcternTypeBits type){
  std::string ret = "";
  switch (type) {
    case arctern::ArcternTypeBits::ARCTERN_FP32:
      return "fp32";
    case arctern::ArcternTypeBits ::ARCTERN_FP16:
      return "fp16";
    case arctern::ArcternTypeBits ::ARCTERN_INT8:
      return "int8";
    default:
      return "fp32";
  }
  return "fp32";
}
ErrorType TrtInterfaceCommonOperation::Init(arctern::ArcternAlgorithmEx *impl, const InitParameter *param) {
  auto p = const_cast<InitParameter*>(param);
  auto ret = impl->CheckInitParam(p);
  if(ret != ErrorType::ERR_SUCCESS) return ret;

  auto cudaErr = cudaSetDevice(p->gpuId_);
  if(cudaErr != cudaSuccess) {
    return ErrorType::ERR_GPU_NO_FIND;
  }
  struct cudaDeviceProp device_prop;
  cudaErr = cudaGetDeviceProperties(&device_prop,p->gpuId_);
  assert(cudaErr == cudaSuccess);

  std::cout << device_prop.name << std::endl;

  assert(isExist(p->model_path));

  bool bTrtModel = false;
  std::vector<std::shared_ptr<TrtEngin>> engines;

  TrtModelInfo model_info;
  model_info.batch = p->batchNum_;
  model_info.modelsPath = model_info.trtModelsPath = p->model_path;

  std::string trtName = "-trt-" + std::to_string(p->batchNum_) + "-" +
      typeBits2string(p->typeBits_) + "-" + string(device_prop.name);
  model_info.trtModelsPath.insert(model_info.trtModelsPath.size()-4,trtName);
  std::string modelPath;
  if(isExist(model_info.trtModelsPath)){
    bTrtModel = true;
    modelPath = model_info.trtModelsPath;
  }
  else{
    modelPath = model_info.modelsPath;
  }
  EngineManager engine_manager;
  engine_manager.parseMxnetModels(modelPath);
  model_info.filebuffer = engine_manager.fileBuffer_;

  model_info.dtype = p->typeBits_ ;
  model_info.gpuId = p->gpuId_;

  unsigned int pos = model_info.trtModelsPath.find_last_of('/');
  model_info.key = model_info.trtModelsPath.substr(pos+1);

  impl->GetModelsConfigInfo(engine_manager.mxnet_models_);
  arctern::TrtEngineManager engineManager(nullptr);

  if(isOnnxModel(model_info.filebuffer->buffer_)){
    engineManager.CreateTrtEngineWithOnnx(&model_info, engines);
  }
  else {
    if(bTrtModel){
      engineManager.CreateEnginWithTrt(&model_info, engines);
    }
    else{
      engineManager.CreateEnginWithMxnet(&model_info, engines);
    }
  }

  impl->SetEngines(engines);
  ret = impl->Init(p);
  if(ret != ErrorType::ERR_SUCCESS) return ret;

  return ErrorType::ERR_SUCCESS;
}
}