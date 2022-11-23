#include "de_infer_engine.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <future>
#include <memory>
#include <chrono>
#include "de_node.h"
#include "de_registry.h"
#include "de_graph_api.h"
#include "de_model_api.h"
#include "de_aiengine_ext_node.h"
#include "de_packed_func.h"
#include "infer_engine_common.h"
#include "sender_host.h"
#include "receiver_host.h"

namespace de {

// 注册全局node
DE_CLASS_REGISTER(HOST_SENDER_CLASS, Sender);
DE_CLASS_REGISTER(HOST_RECEIVER_CLASS, Receiver);

InferEngine::InferEngine(const std::string &net_bin_path, const std::string &model_bin_path, int dev_id, int max_batch) {
  // 加载模型
  dev_id_ = dev_id;
  mm_ = new ModelManager(dev_id);
  std::string net_bin_ = "host:" + net_bin_path;
  std::string model_bin = "host:" + model_bin_path;
  mm_->Load(net_bin_, model_bin, false);
  LOG(IMPORTANT) << "model load complete: " << net_bin_;

  // 创建graph
  graph_ = new de::Graph("infer_engine");

  // 创建Bridge
  // par->graph->CreateBridge(0, "192.168.1.100", 9200);
  graph_->CreateBridge(dev_id_);

  std::map<std::string, int> attr;

  // 关闭crc校验
  attr["need_check_crc"] = 0;

  graph_->AddBridgeH2DChan(0, "Host2Device", attr);
  graph_->AddBridgeD2HChan(1, "Device2Host", attr);

  // 创建node
  graph_->CreateHostNode(HOST_SENDER_CLASS,   HOST_SENDER_NODE);
  graph_->CreateHostNode(HOST_RECEIVER_CLASS, HOST_RECEIVER_NODE);

  graph_->CreateDevNode(DEVICE_SENDER_CLASS,   DEVICE_SENDER_NODE);
  graph_->CreateDevNode(DEVICE_RECEIVER_CLASS, DEVICE_RECEIVER_NODE);
  graph_->CreateDevNode(DEVICE_AIENGINE_CLASS, DEVICE_AIENGINE_NODE);
  graph_->CreateDevNode("de::JpegDecoder", "JpegDecNode");

  // 设置node属性
  graph_->SetNodeAttr(DEVICE_AIENGINE_NODE, "model_net_path", net_bin_);
  graph_->SetNodeAttr(DEVICE_AIENGINE_NODE, "model_param_path", model_bin);
  graph_->SetNodeAttr(DEVICE_AIENGINE_NODE, "batch_num", max_batch);

  // 设置graph内node连接
  graph_->LinkNode(HOST_SENDER_NODE, 0, DEVICE_RECEIVER_NODE, 0);

  graph_->LinkNode(DEVICE_RECEIVER_NODE, 0, "JpegDecNode");
  graph_->LinkNode("JpegDecNode", 0, DEVICE_AIENGINE_NODE);
  graph_->LinkNode(DEVICE_RECEIVER_NODE, 1, DEVICE_AIENGINE_NODE);
  graph_->LinkNode(DEVICE_AIENGINE_NODE, 0, DEVICE_SENDER_NODE);

  graph_->LinkNode(DEVICE_SENDER_NODE, 0, HOST_RECEIVER_NODE, 1);

  // 设置graph输入和输出
  graph_->SetInputNode(0, HOST_SENDER_NODE);
  graph_->SetOutputNode(0, HOST_RECEIVER_NODE, 0);

  // 启动业务
  graph_->Start();
  LOG(INFO) << "Graph start...";
}

InferEngine::~InferEngine() {
  // 删除graph
  LOG(INFO) << "Graph destruct...";
  if (graph_) {
    // std::future_status status;
    // do {
    //   status = result_.wait_for(std::chrono::seconds(1));
    // } while (status != std::future_status::ready);

    graph_->StopOutput<de::NNTask>(0);
    delete graph_;
    if (mm_) {
      mm_->Unload(net_bin_);
      LOG(IMPORTANT) << "model unload complete: " << net_bin_;
      delete mm_;
    }
  }

  NodeClassRegistry::Remove(HOST_SENDER_CLASS);
  NodeClassRegistry::Remove(HOST_RECEIVER_CLASS);

  // 卸载模型
  ModelManager mem(dev_id_);
  std::string net_bin = "host:" + net_bin_;
  mem.Unload(net_bin);
}

void InferEngine::Predict(std::vector<de::NDArray> &ins, std::vector<de::NDArray> &outs) {
  static int32_t id = 0;
  int tensor_num = ins.size();
  // 将tensor拆分发送，以实现零拷贝
  for (int i = 0; i < tensor_num; i++) {
    auto task = new SenderTask;
    task->total = tensor_num;
    task->id = id;
    task->array = ins[i];
    //printf("SetInput total %d, id %d\n", task->total, task->id);
    graph_->SetInput(0, task);
  }
  
  auto result = *(static_cast<de::NNTask*>(graph_->GetOutput(0)));
  out_task_.id = result.id;
  out_task_.tensors = result.tensors;
  if (id != out_task_.id) LOG(ERROR) << "Predict receive id " << out_task_.id << " is not send id " << id;
  id++;
  outs = out_task_.tensors;
}

InferData InferEngine::GetOutput() {
  return out_task_;
}

void* LoadLibrary(int dev_id, const std::string &lib_path) {
  void *handle = de::Device::LoadLib(dev_id, lib_path);
  LOG(IMPORTANT) << "dev " << dev_id << " load library "<< handle << " success.";
  return handle;
}

void UnloadLibrary(int dev_id, void* handle) {
  de::Device::UnLoadLibHandle(dev_id, handle);
  LOG(IMPORTANT) << "dev " << dev_id << " unload library "<< handle << " success.";
}

std::unordered_map<int, InferEngine*> engine_map;

int de_infer_engine_create(const char *net_bin_path, const char *model_bin_path, int dev_id, int max_batch) {
  std::string net_file = net_bin_path;
  std::string model_file = model_bin_path;
  static int id = 0;
  auto engine = new de::InferEngine(net_file, model_file, dev_id, max_batch);
  engine_map.emplace(id, engine);
  return id++;
}

int de_infer_engine_destroy(int handle) {
  std::unordered_map<int, InferEngine*>::const_iterator iter = engine_map.find(handle);
  if (iter == engine_map.end()) {
    LOG(ERROR) << "InferEngine destroy error. engine not found.";
    return 1; // 没有找到handle
  } else {
    delete iter->second;
    engine_map.erase(iter);
  }
  return 0;
}

void de_infer_engine_predict(de::NDArray &in, std::vector<de::NDArray> &out) {

}

// python接口封装

DE_REGISTER_GLOBAL("de.infer_engine.create").
  set_body([](de::DEArgs args, de::DERetValue *rv) {
    *rv = new InferEngine(args[0], args[1], args[2], args[3]);
  });

DE_REGISTER_GLOBAL("de.infer_engine.predict").
  set_body([](de::DEArgs args, de::DERetValue *rv) {
    auto engine = args[0].ptr<de::InferEngine>();
    int input_num = args[1];
    int batch_num = args[2];
    int nd_num = input_num * batch_num;
    std::vector<de::NDArray> ins, outs;
    for (int i = 0; i < nd_num; i++) {
      //de::NDArray in = args[3 + i];
      de::NDArray in(args[3 + i].ptr<de::NDArray::Container>());
      ins.emplace_back(in);
    }

    // LOG(INFO) << "engine->Predict input_num " << input_num << ", batch_num " << batch_num;

    engine->Predict(ins, outs);
    *rv = (int)outs.size();
  });

DE_REGISTER_GLOBAL("de.infer_engine.get_output").
  set_body([](de::DEArgs args, de::DERetValue *rv) {
    auto engine = args[0].ptr<de::InferEngine>();
    *rv = engine->GetOutput().tensors[args[1]];
  });

DE_REGISTER_GLOBAL("de.infer_engine.destroy").
  set_body([](de::DEArgs args, de::DERetValue *rv) {
    auto engine = args[0].ptr<de::InferEngine>();
    delete engine;
    *rv = 0;
  });

DE_REGISTER_GLOBAL("de.load_library").
  set_body([](de::DEArgs args, de::DERetValue *rv) {
    *rv = LoadLibrary(args[0], args[1]);
  });

DE_REGISTER_GLOBAL("de.unload_library").
  set_body([](de::DEArgs args, de::DERetValue *rv) {
    UnloadLibrary(args[0], args[1]);
  });

DE_REGISTER_GLOBAL("de.to_ndarray").
  set_body([](de::DEArgs args, de::DERetValue *rv) {
    DEByteArray* byte_array = args[3].ptr<DEByteArray>();
    int format = args[0];
    if (format == DE_PIX_FMT_JPEG) {
      *rv = de::NDArray::Create(args[2], 1, format, {0, 0, 0, 0}, const_cast<char*>(byte_array->data));
    } else {
      *rv = de::NDArray::Create(args[2], args[1], format, {0, 0, args[2], args[1]}, const_cast<char*>(byte_array->data));
    }
  });

}
