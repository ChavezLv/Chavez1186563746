#include <iostream>
#include <thread>
#include <fstream>
#include "de_node.h"
#include "de_graph_api.h"
#include <de_thread.h>
#include "de_pic_encoder_node.h"
#include "de_node_common.h"

typedef struct{
  std::string yuv_in;
  std::string jpeg_out;
  int width;
  int height;
  dePixelFormat format; 
}TestCfg;

static TestCfg kCaseCfg = {
  yuv_in   : "/DEngine/data/1080p/1080p.yuv",
  jpeg_out  : "./jpeg_enc_out.jpg",
  width     : 1920,
  height    : 1080,
  format    : DE_PIX_FMT_NV12,
}; 

class Sender : de::Thread {
  public:
  Sender(){
    pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
  }
  void Proc(void* rx_task, int32_t task_type, POutType pout_type){
    de::MediaTask* pTask = static_cast<de::MediaTask*>(rx_task);
    pouts_[0]->SendTask(pTask);
  }
  void Stop(void){pin.DestroyQueue();}
};
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
namespace de
{
DE_CLASS_REGISTER("Sender", Sender);
DE_CLASS_REGISTER("Receiver", Receiver);
}

de::NDArray get_image(dePixelFormat fmt,int width,int height, const char* pic_path = nullptr, const char* pic_data = nullptr) {
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
	// image.SaveTensorInfo();

	if (pic_path) {
    std::string file = pic_path;
		std::ifstream f(file,std::ios_base::binary|std::ios_base::out);
		if(f) {
			f.read(reinterpret_cast<char*>(image.GetTensorData()),image.GetTensorDataSize());
			f.close();
		}	
	}else if (pic_data) {
    memcpy((char*)image.GetTensorData(), pic_data, image.GetTensorDataSize());
	}
	return image;
}

int main(int argc, char *argv[]) {
  std::string url="192.168.33.86";
  int port= 9200;
  
  //创建graph
  de::Graph graph("jpeg_enc");
  
  //创建bridge，连接输入和解码2个node
    if (argc >= 3) {
    url = argv[1];
    port = atoi(argv[2]);
    graph.CreateBridge(0, url, port);
  }else
    graph.CreateBridge(0);
  graph.AddBridgeH2DChan(0, "Host2Device");
  graph.AddBridgeD2HChan(1, "Device2Host");

  //创建node
  graph.CreateHostNode("Sender", "yuv_in");
  graph.CreateHostNode("Receiver", "jpeg_out");
  graph.CreateDevNode("de::JpegEncoder", "jpeg_enc");

  //设置graph内node连接，yuv_in的Pout[0]连接到jpeg_enc的Pin，使用bridge通道0
  graph.LinkNode("yuv_in", 0, "jpeg_enc", 0);
  //设置graph内node连接，jpeg_enc的Pout[0]连接jpeg_out的Pin，使用bridage通道1
  graph.LinkNode("jpeg_enc", 0, "jpeg_out", 1);

  //设置graph输出，graph的Pout[0]为jpeg_out的Pout[0]
  graph.SetOutputNode(0, "jpeg_out", 0);

  //设置graph输入，graph的Pin[0]为jpeg_in的Pin
  graph.SetInputNode(0, "yuv_in");
  //启动业务
  graph.Start();

  //启动结果读取线程，获取graph执行结果
  std::thread([](de::Graph *graph) {
    void *p;
    while (nullptr != (p = graph->GetOutput(0))) {
      de::MediaTask *task = static_cast<de::MediaTask*>(p);
      LOG(INFO) << "yuv ====> jpeg " << " encode finished.";
      FILE *file = fopen(kCaseCfg.jpeg_out.c_str(), "w");
      fwrite((char*)task->array.GetTensorData(), task->array.GetTensorDataSize(), 1, file);
      delete task;
    }
  }, &graph).detach();

  //输入yuv数据
  de::NDArray image = get_image(kCaseCfg.format, kCaseCfg.width, kCaseCfg.height,  kCaseCfg.yuv_in.c_str(), nullptr);
  for (int i = 0; i < 1; i++) {
    de::MediaTask* p = new de::MediaTask;
    p->array = image;
    p->streamid = 1234;
    p->pts = 5555;
	p->flag = 0;
    graph.SetInput(0, p);
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));
  //确保graph的output队列退出
  graph.StopOutput<de::MediaTask>(0);
  return 0;
}
