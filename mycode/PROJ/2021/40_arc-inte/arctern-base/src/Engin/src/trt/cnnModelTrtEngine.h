//
// Created by chengaohua on 2021/7/8.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_TRT_CNNMODELTRTENGINE_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_TRT_CNNMODELTRTENGINE_H_

#include "core/trtInfer.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/file_buffer.h"

namespace arctern {


///\brief TRT engine Class wrapper
class CnnModelTRTEngine {
 public:
  explicit CnnModelTRTEngine(int gpu_id = -1, ArcternTypeBits model_dtype = ArcternTypeBits::ARCTERN_FP32, int max_BatchSize = 1) {
    trtEngine_ = std::make_shared<TrtInferEngine>(gpu_id, model_dtype, max_BatchSize);
  }

  ~CnnModelTRTEngine() = default;

  /// init engine by deserializeCudaEngine
  bool init(const FileBuffer &file_buffer);
  bool init(const MxnetModel *net_model);

  ///\brief init engine by buildEngine and serialize trt model to bin
  ///\param file_buffer: mxnet model
  ///\param configs_str: the config info string
  ///\param options: for flatbuff optional config
  bool init(const FileBuffer &file_buffer, const std::string &trt_models_path,
            std::vector<std::pair<std::string, std::string>> options = std::vector < std::pair < std::string,
            std::string
  >>());

  bool initOnnx(const std::string& onnxModelPath,
                const std::string& trtModelPath,
                const FileBuffer& file_buffer);

  /// destory engine
  void destory();

  /// is input index for binding context
  bool isBindingInputIndex(int index);

  /// get the dims of index in bindins
  std::vector<int> getDimPerBinding(int index);

  /// get the number of bindings
  int getNbBindings();

  /// get the size of per binding
  size_t getSizePerBinding(int index, int batchSize = 1);

  /// get the maximum batch size which can be used for inference.
  int getMaxBatchSize();

  /// get the version of trt model
  std::string get_version_string() {
    return version_;
  }

  std::array<int, 3> get_version() {
    std::vector<std::string> versions;
    //split(version_, ".", versions);
    assert(versions.size() == 3);
    //todo
    return std::array<int, 3>({0, 0, 0});
  }

  /// get the engine of trt backend
  ICudaEngine *getTrtEngineInstance() {
    return trtEngine_->engine_;
  }

  ///\brief serial engine, convert trt model to FlatBuffer
  ///\return string contain trtModelBin info
  void convertTrtModelBin(const FileBuffer &file_buffer,const string& trtModelsPath);

  void SaveTrtModel(const string& trtModelPath);

 private:
  ///\brief the implement of init
  ///\param options: for flatbuff optional config
  bool initImpl(const FileBuffer &file_buffer, std::vector<std::pair<std::string, std::string>> &options);

 private:
  ///configs_str_: for the convertTrtModelBin interface, the config param will be writed to bin
  std::string configs_str_;
  std::string version_; /// the version of trt model file
  /// for flatbuff optional config, it's dynamic range cache for INT8 Model currently
  std::vector<std::pair<std::string, std::string>> options_;

  const int *inputDims_ = nullptr; /// the input dims of trt model file
  std::shared_ptr<TrtInferEngine> trtEngine_; /// trt engine
};

}

#endif //ARCTERN_BASE_SRC_ENGIN_SRC_TRT_CNNMODELTRTENGINE_H_
