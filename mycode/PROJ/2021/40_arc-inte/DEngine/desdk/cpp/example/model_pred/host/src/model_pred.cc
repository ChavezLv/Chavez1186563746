#include <iostream>
#include <thread>
#include <fstream>
#include "de_node.h"
#include "de_graph_api.h"
#include "de_model_api.h"
#include "de_aiengine_node.h"

namespace de {

/// @brief 发送者node，用于将数据发送给芯片
class Sender : public de::Thread
{
 public:
  Sender() {
    //设置回调函数类型，这里只需要根据传递的类型替换de::NNtask
    pin.SetTypeInfo(0, "de::NNTask", de::TaskDeSerialize<de::NNTask>, de::TaskDeleter<de::NNTask>);
    pouts_[0]->SetTypeInfo("de::NNTask", de::TaskSerialize<de::NNTask>);
  }
  ~Sender() {
    //删除入队列，停止算子
    pin.DestroyQueue();

    //等待队列线程退出
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type) {
    pouts_[0]->SendTask(rx_task);
  }
  void Stop(void){pin.DestroyQueue();}
};

/// @brief 接收者node，用于接收芯片侧结果返回
class Receiver : public de::Thread
{
 public:
  Receiver() {
    //设置回调函数类型，这里只需要根据传递的类型替换de::NNtask
    pin.SetTypeInfo(0, "de::NNTask", de::TaskDeSerialize<de::NNTask>, de::TaskDeleter<de::NNTask>);
    pouts_[0]->SetTypeInfo("de::NNTask", de::TaskSerialize<de::NNTask>);
  }
  ~Receiver() {
    //删除入队列，停止算子
    pin.DestroyQueue();
    pouts_[0]->DestroyQueue<de::NNTask>();

    //等待队列线程退出
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type) {
    pouts_[0]->SendTask(rx_task);
  }
  void Stop(void){pin.DestroyQueue();}
};

//注册全局node
DE_CLASS_REGISTER("de::Sender", Sender);
DE_CLASS_REGISTER("de::Receiver", Receiver);



de::NDArray get_image(dePixelFormat fmt, int width, int height, const char *pic_path = nullptr, const char *pic_data = nullptr) {
  int real_width = width;
  int real_height = height;
  std::vector<int64_t> shape;
  ExtInfo addition;
  switch (fmt)
  {
    case dePixelFormat::DE_PIX_FMT_NV12:
    case dePixelFormat::DE_PIX_FMT_NV21:
      real_height = height*3/2;
      break;
    case dePixelFormat::DE_PIX_FMT_RGB888:
    case dePixelFormat::DE_PIX_FMT_BGR888:
      real_height *= 3;
      break;
    case dePixelFormat::DE_PIX_FMT_NV16:
    case dePixelFormat::DE_PIX_FMT_YUV422_UYVY:
    case dePixelFormat::DE_PIX_FMT_YUV422_YUYV:
      real_height *= 2;
      break;
    case dePixelFormat::DE_PIX_FMT_GRAY8:
      real_height *= 1;
      break;
    default:
      break;
  }
  
  shape.push_back(real_height);
  shape.push_back(real_width);
  addition.shape_indicator = deShapeCode::HW;
  
  addition.format = fmt;
  addition.bear_data_len = 0;
  addition.pts = 0;
  addition.roi_rect = {0,0,width,height}; 
  de::NDArray image = de::NDArray::Empty(shape, {DLDataTypeCode::kDLUInt, 8, 1},
                                {DLDeviceType::kDLCPU, 0}, addition);
  image.SaveTensorInfo();

  if (pic_path) {
    std::string file = pic_path;
    std::ifstream f(file, std::ios_base::binary | std::ios_base::out);
    if(f) {
      f.read(reinterpret_cast<char*>(image.GetTensorData()), image.GetTensorDataSize());
      f.close();
    } 
  } else if (pic_data) {
    memcpy((char*)image.GetTensorData(), pic_data, image.GetTensorDataSize());
  }
  return image;
}
}


int main(int argc, char *argv[]) {

  std::string url="192.168.33.86";
  int port= 9200;

  //创建graph
  de::Graph graph("model_pred");
  
  //创建bridge，连接取流和解码2个node
  if (argc >= 3) {
    url = argv[1];
    port = atoi(argv[2]);
    graph.CreateBridge(0, url, port);
  }else
    graph.CreateBridge(0);
  
  graph.AddBridgeH2DChan(0, "Host2Device");
  graph.AddBridgeD2HChan(1, "Device2Host");

  //加载模型到芯片内存
  de::ModelManager mem(0);

  std::string net_bin_path = "/DEngine/model/dp1000/caffe_squeezenet_v1.1/net.bin";
  std::string model_bin_path = "/DEngine/model/dp1000/caffe_squeezenet_v1.1/model.bin";
  std::string net_bin = "host:" + net_bin_path;
  std::string model_bin = "host:" + model_bin_path;
  mem.Load(net_bin, model_bin, false);

  //创建node
  graph.CreateHostNode("de::Sender", "Sender");
  graph.CreateDevNode("de::AiEngine", "AiEngine");
  graph.CreateHostNode("de::Receiver", "Receiver");

  //设置node属性
  graph.SetNodeAttr("AiEngine", "model_net_path", net_bin);
  graph.SetNodeAttr("AiEngine", "model_param_path", model_bin);
  graph.SetNodeAttr("AiEngine", "resize_type", 0);
  graph.SetNodeAttr("AiEngine", "batch_num", 8);
  // graph.SetNodeAttr("AiEngine", "batch_now", 1);

  //设置graph内node连接
  graph.LinkNode("Sender", 0, "AiEngine", 0);
  graph.LinkNode("AiEngine", 0, "Receiver", 1);

  //设置graph输入和输出
  graph.SetInputNode(0, "Sender");
  graph.SetOutputNode(0, "Receiver", 0);

  //启动业务
  graph.Start();

  //启动结果读取线程，获取graph执行结果
  std::thread([](de::Graph *_graph) {

    void *p;
    while (nullptr != (p = _graph->GetOutput(0))) {
      de::NNTask *task = static_cast<de::NNTask*>(p);
      LOG(INFO) << "<====AiEngine result " << task->id << " size " << task->tensors.size();
      delete task;
    }

  }, &graph).detach();

  std::thread([](de::Graph *_graph) {
    //启动送数据线程，将数据送给AiEngine
    std::vector<int64_t> shape;
    ExtInfo addition;
    shape.push_back(320);
    shape.push_back(320);
    addition.shape_indicator = deShapeCode::HW;
    
    int pts = 0;
    addition.format = DE_PIX_FMT_BGR888_PLANE;
    addition.bear_data_len = 0;
    addition.roi_rect = {0, 0, 320, 320};
    
    for (int i = 0; i < 100; i++) {
      auto task = new de::NNTask;
      
      addition.pts = pts++;
      de::NDArray data = de::NDArray::Empty(shape, {DLDataTypeCode::kDLUInt, 8, 1},
                                {DLDeviceType::kDLCPU, 0}, addition);
      memset(data.GetTensorData(), 0x33, 320*320);
      
      task->tensors.push_back(data);
      task->id = i;
      LOG(INFO) << "====>graph input i = " << i;
      _graph->SetInput(0, task);
    }
  }, &graph).join();

  std::this_thread::sleep_for(std::chrono::seconds(20));
  //确保graph的output队列退出
  graph.StopOutput<de::NNTask>(0);

  mem.Unload(net_bin);

  return 0;
}

