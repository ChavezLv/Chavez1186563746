//
// Created by chengaohua on 2021/7/8.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_TRT_CNNMODELTRTCONTEXT_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_TRT_CNNMODELTRTCONTEXT_H_

#include "core/trtInfer.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/file_buffer.h"
#include "cnnModelTrtEngine.h"


namespace arctern {
///\brief TRT context Class wrapper
class CnnModelTRTContext {
 public:
  CnnModelTRTContext(int gpu_id = -1, int batchSize = 1);

  ~CnnModelTRTContext();

  /// init the context
  bool init(CnnModelTRTEngine *trtEngine);

  /// destroy the context
  void destory();

  /// init outputs for face-det's pnetDet
  bool initOutputsZoom(std::pair<float *, size_t> *outputs, int outputSize);

  void *getPtrOfTrtInputLayer();

  void getSizePerBindings(std::vector<size_t> &perSize, int batchSize);

  size_t getSizeBindingByIndex(int index, int batchSize);

  void alloc_bindings(int batchSize);

  cudaStream_t *getCudaStream() {
    return stream;
  }

  /// do forword
  std::vector<std::vector<float>> forward();
  int forward(std::vector<std::vector<float>> &outputs);
  /// do forword for face-det's pnetDet
  int forward(std::pair<float *, size_t> *outputs, int outputSize);

 public:
  int currentBatchSize_ = 1; /// Default, currentBatchSize_ = 1;
  int maxBatchSize_;

 private:
  /// do inference impl
  int forwardImpl(void **bindings, std::vector<std::vector<float>> &outputs, int batchSize);

  /// do inference for face-detect's pnetDet impl
  int forwardImpl(void **bindings, std::pair<float *, size_t> *outputs, int outputSize, int batchSize);

 private:
  ///> Dynamic shape onnx
  bool m_bDynamic = false;
  cudaStream_t *stream = nullptr; /// cuda stream
  int nbBindings_; /// the inputs and outputs numbers of bindings for trt
  std::vector<std::vector<int>>  nbDims_; // the inputs and outputs shape
  std::vector<void *> bindings_; /// the device memory of bindings for inputs and outputs

  //CnnModelTRTEngine *engine_ = nullptr; /// trt backend infer engine
  IExecutionContext *context_ = nullptr; /// trt batend infer context

};

}

#endif //ARCTERN_BASE_SRC_ENGIN_SRC_TRT_CNNMODELTRTCONTEXT_H_
