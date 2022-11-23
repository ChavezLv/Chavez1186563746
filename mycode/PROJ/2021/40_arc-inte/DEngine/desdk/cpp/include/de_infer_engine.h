#pragma once

#include "de_ndarray.h"
#include <fstream>
#include <vector>
#include <thread>

namespace de {

class Graph;
class ModelManager;

struct InferData
{
  uint64_t id;
  std::vector<de::NDArray> tensors;
};

class InferEngine {

 public:
  InferEngine(const std::string &net_bin_path, const std::string &model_bin_path, int dev_id = 0, int max_batch = 1);

  ~InferEngine();

  void Predict(std::vector<NDArray> &ins, std::vector<NDArray> &outs);

  InferData GetOutput();

 protected:
  int dev_id_ = -1;
  int input_num_ = 1;                    // 模型输入个数，上限4
  int input_que_size_;                   // 输入队列大小，上限32

  Graph *graph_ = nullptr;
  ModelManager *mm_ = nullptr;
  std::string net_bin_;

  // std::future<int64_t> result_;         // 最后一张图片推理future
  InferData out_task_;                 // 保存最近一次推理结果
};

void* LoadLibrary(int dev_id, const std::string &lib_path);

void UnloadLibrary(int dev_id, void* handle);

}