#include <iostream>
#include <thread>
#include "de_node.h"
#include "de_graph_api.h"
#include "de_video_in_node.h"
#include "de_video_decoder_node.h"
#include "de_pic_encoder_node.h"
#include "de_node_common.h"

using namespace de;
using namespace de::mm;

//接收node，从设备node透传数据
class Receiver : de::Thread {
  public:
  Receiver(){
    pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
  }
  void Proc(void* rx_task, int32_t task_type, POutType pout_type){
    de::MediaTask* pTask = static_cast<de::MediaTask*>(rx_task);
    pouts_[0]->SendTask(pTask);
  }
  void Stop(void){pin.DestroyQueue();}
};

DE_CLASS_REGISTER("Receiver", Receiver);


int main(int argc, char **argv){
  de::InitLogLevel(0);
  //创建graph
  de::Graph graph("video_dec");

  //创建bridge，连接取流和解码2个node
  std::string url="192.168.33.86";
  
  //视频源地址，可以是RTSP流或本地视频文件
  std::string video_uri = "rtsp://admin:introcks1234@192.168.33.124";
  // std::string video_uri = "file:///DEngine/desdk/data/video/person1.h264";
  
  int port= 9200;
  if (argc >= 3) {
    url = argv[1];
    port = atoi(argv[2]);
	if (argc >= 4) video_uri = argv[3];
    graph.CreateBridge(0, url, port);
  }
  else{
    graph.CreateBridge(0);
    if (argc == 2) video_uri = argv[1];
  }
  graph.AddBridgeH2DChan(0, "Host2Device");
  graph.AddBridgeD2HChan(1, "Device2Host");

  //创建node
  graph.CreateHostNode("de::VideoInput", "video_in");
  graph.CreateDevNode("de::VideoDecoder", "video_dec");
  graph.CreateHostNode("Receiver", "Receiver");

  //设置node属性
  graph.SetNodeAttr("video_in", "uri", video_uri);
  graph.SetNodeAttr("video_in", "stream-id", 1);
  graph.SetNodeAttr("video_in", "width", 1920);
  graph.SetNodeAttr("video_in", "height", 1080);
  graph.SetNodeAttr("video_in", "framerate", 25.0);
  graph.SetNodeAttr("video_in", "lower-trans-type", 0);
  graph.SetNodeAttr("video_in", "stream-type", DE_PIX_FMT_H264);
  graph.SetNodeAttr("video_dec", "output-buffers", 16);
  graph.SetNodeAttr("video_dec", "stream-type", DE_PIX_FMT_H264);

  //设置graph内node连接，video_in的pouts_[0]连接video_dec的Pin，使用bridge通道0
  graph.LinkNode("video_in", 0, "video_dec", 0);
  //设置graph内node连接，video_in的pouts_[0]连接video_dec的Pin，使用bridge通道0
  graph.LinkNode("video_dec", 0, "Receiver", 1);

  //设置graph输入和输出，graph的pouts_[0]为video_dec的pouts_[0]
  graph.SetOutputNode(0, "Receiver", 0);

  //启动业务
  graph.Start();

  //启动结果读取线程，获取graph执行结果
  std::thread([](de::Graph *graph) {
    void *p;
    while (nullptr != (p = graph->GetOutput(0))) {
      de::MediaTask *task = static_cast<de::MediaTask*>(p);
      LOG(INFO) << "====>stream " << task->streamid << " frame " << task->pts <<" decode finished.";
      delete task;
    }
  }, &graph).detach();

  std::this_thread::sleep_for(std::chrono::seconds(5));
  //确保graph的output队列退出
  graph.StopOutput<de::MediaTask>(0);

}
