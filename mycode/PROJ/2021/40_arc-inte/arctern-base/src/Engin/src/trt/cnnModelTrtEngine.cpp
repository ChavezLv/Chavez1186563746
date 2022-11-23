//
// Created by chengaohua on 2021/7/8.
//

#include "cnnModelTrtEngine.h"
#include "core/convert/convert_trtModel.h"
#include "common/common.h"
#include <numeric>

namespace arctern {
///************************ the implement of CnnModelTRTEngine ************************
bool CnnModelTRTEngine::init(const FileBuffer &file_buffer) {
  auto trt_models = GetMxnetModels(file_buffer.buffer_);
  auto models = trt_models->models();
  auto net_model = const_cast<MxnetModel *>(models->Get(0));

  return init(net_model);
}

bool CnnModelTRTEngine::init(const MxnetModel *net_model) {
  // std::cout << net_model << std::endl;
  // std::cout<<net_model->version()->str()<<std::endl;
  // version_ = net_model->version()->str();

  auto ret = trtEngine_->initTRT(net_model->params()->data(), net_model->params()->size());
  return ret;
}

bool CnnModelTRTEngine::init(const FileBuffer &file_buffer, const std::string &trt_models_path,
                             std::vector<std::pair<std::string, std::string>> options) {
  /// init TRT engine
  bool initEngineOk = initImpl(file_buffer, options);
  ///>save trt models
  convertTrtModelBin(file_buffer,trt_models_path);
  return true;
}

void CnnModelTRTEngine::destory() {
  trtEngine_->destoryTRT();
}

bool CnnModelTRTEngine::isBindingInputIndex(int index) {
  return trtEngine_->engine_->bindingIsInput(index);
}

std::vector<int> CnnModelTRTEngine::getDimPerBinding(int index) {
  Dims dims = trtEngine_->engine_->getBindingDimensions(index);
  std::vector<int> vdims;
  for (int i = 0; i < dims.nbDims; ++i) {
    vdims.push_back(dims.d[i]);
  }

  return vdims;
}

int CnnModelTRTEngine::getNbBindings() {
  return trtEngine_->engine_->getNbBindings();
}

size_t CnnModelTRTEngine::getSizePerBinding(int index, int batchSize) {
  Dims dims = trtEngine_->engine_->getBindingDimensions(index);
  size_t size = accumulate(dims.d, dims.d + dims.nbDims, batchSize, multiplies<size_t>());

  return size;
}

int CnnModelTRTEngine::getMaxBatchSize() {
  return trtEngine_->engine_->getMaxBatchSize();
}


bool CnnModelTRTEngine::initOnnx(const std::string& onnxModelPath,
                                 const std::string& trtModelPath,
                                 const FileBuffer& file_buffer){
  auto ret = false;

  auto models = GetMxnetModels(file_buffer.buffer_);
  auto model = models->models()->Get(0);
  auto data = model->params();

  if(!isExist(trtModelPath)){
    ret = trtEngine_->initOnnx(static_cast<const void*>(data->data()),data->size());
    //SaveTrtModel(trtModelPath);
    convertTrtModelBin(file_buffer,trtModelPath);
  }
  else{
    ret = trtEngine_->initTRT(static_cast<const void*>(data->data()),data->size());;
  }
  return ret;
}

bool CnnModelTRTEngine::initImpl(const FileBuffer &file_buffer,
                                 std::vector<std::pair<std::string, std::string>> &options) {
  /// save needed options for convertTrtModelBin
  options.swap(options_);

  /// parse the mxnet file_buffer to get the config info
  auto mxnet_models = GetMxnetModels(file_buffer.buffer_);
  auto models = mxnet_models->models();
  auto net_model = const_cast<MxnetModel *>(models->Get(0));

  version_ = mxnet_models->version()->str();
  inputDims_ = net_model->input_shape()->data();
  nvinfer1::Dims inputDims;
  inputDims.nbDims = 4;
  inputDims.d[0] = inputDims_[0];
  inputDims.d[1] = inputDims_[1];
  inputDims.d[2] = inputDims_[2];
  inputDims.d[3] = inputDims_[3];
  auto net_json = net_model->json();
  auto net_params = net_model->params();
  auto net_configs = net_model->configs();

  /// build trtEngine
  if (trtEngine_->model_dtype_ == 0 || trtEngine_->model_dtype_ == 1) {
    /// F32 model format or F16 model format
    return trtEngine_->initTRT(net_json->c_str(), net_params->data(), inputDims);
  } else if (trtEngine_->model_dtype_ == 2) {
    /// [option]:get INT8ModeCache
    char *int8ModelCache;
    size_t int8ModelCacheSize;
    bool getOk = try_get_opt(*mxnet_models, "INT8Cache", &int8ModelCache, int8ModelCacheSize);
//            ARCTERN_ASSERT_MSG(getOk, "get INT8 Model Cache failed!");
    std::string dynamicRangeCache(int8ModelCache, int8ModelCacheSize);
    auto opt1 = std::make_pair(std::string("INT8Cache"), dynamicRangeCache);
    options_.emplace_back(opt1);
    delete[] int8ModelCache; /// need manual free

    char *layerPrecisionCache;
    size_t layerPrecisionCacheSize;
    getOk = try_get_opt(*mxnet_models, "LayerPrecision", &layerPrecisionCache, layerPrecisionCacheSize);
    std::string layerPrecision = std::string();
    if (getOk) {
      layerPrecision = std::string(layerPrecisionCache, layerPrecisionCacheSize);
      auto opt2 = std::make_pair(std::string("LayerPrecision"), layerPrecision);
      options_.emplace_back(opt2);
      delete[] layerPrecisionCache; /// need manual free
    }
    return trtEngine_->initTRT(net_json->c_str(), net_params->data(), inputDims, dynamicRangeCache, layerPrecision);
  }
}

void CnnModelTRTEngine::SaveTrtModel(const string& trtModelPath){
  unique_ptr<IHostMemory, arctern::trtParser::Destroy<IHostMemory>> enginePlan{trtEngine_->engine_->serialize()};
  std::ofstream fout(trtModelPath,ios_base::out|ios_base::binary|ios_base::trunc);
  if(!fout.is_open()){
    std::cout << "open file failed,when save trt model,file="<<trtModelPath<<endl;
    return;
  }
  fout.write(static_cast<char*>(enginePlan->data()),enginePlan->size());
  fout.close();
}

void CnnModelTRTEngine::convertTrtModelBin(const FileBuffer &file_buffer,const string &trtModelsPath) {
  /// serial engine
  unique_ptr<IHostMemory, arctern::trtParser::Destroy<IHostMemory>> enginePlan{trtEngine_->engine_->serialize()};
  int maxBatchSize = getMaxBatchSize();
  auto mxnetModels = GetMxnetModels(file_buffer.buffer_);
  auto  models = mxnetModels->models();
  auto opt = mxnetModels->opts();
  auto model = models->Get(0);
  auto config = model->configs();
  int configSize=0;
  std::string configStr="";
  if(config != nullptr){
    configSize = config->size();
    for(auto it = config->begin();it != config->end(); it++){
      configStr+=it->key()->str()+"="+it->value()->str();
      configStr+=":";
    }
  }
  if(opt != nullptr){
    options_.clear();
    for(auto it = opt->begin();it!=opt->end();it++)
    {
      options_.push_back(make_pair(it->key()->str(),string((char*)it->bytes()->data(),it->bytes()->size())));
    }
  }
  /// convertTool interface for convert trt to FileBuff
    arctern::convert::convertTrt2Bin((char*)(enginePlan->data()),(int) enginePlan->size(), version_,
                                   maxBatchSize, configStr, options_,trtModelsPath);

}
}