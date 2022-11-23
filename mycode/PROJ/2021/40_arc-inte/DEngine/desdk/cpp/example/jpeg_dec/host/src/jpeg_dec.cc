#include <iostream>
#include <thread>
#include <fstream>
#include "de_node.h"
#include "de_graph_api.h"
#include <de_thread.h>
#include "de_pic_decoder_node.h"
#include "de_node_common.h"

using namespace std;


#define DEBUG_ABORT_TRACE_ON
#ifdef DEBUG_ABORT_TRACE_ON
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#define SIZE 1000

#if !defined(gettid)
# define gettid() syscall(__NR_gettid)
#endif 
#include <chrono>
void trace(int signo)
{
    int j, nptrs;
    void *buffer[SIZE];
    char **strings;
    char cName[32];
    prctl(PR_GET_NAME, cName);
    printf("Comm: %s, PID:%ld, signo: %d\n", cName, gettid(), signo);
    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);
    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
     *               would produce similar output to the following: */
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("Backtrace:");
        exit(EXIT_FAILURE);
    }
    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);
    free(strings);
    if (SIGSEGV == signo || SIGQUIT == signo ||SIGABRT == signo) {
        signal(signo, SIG_DFL);
        raise(signo);
    }
}
#endif




typedef struct{
  std::string jpeg_in;
  std::string file_out;
  int width;
  int height;
  dePixelFormat format; 
}TestCfg;

static TestCfg kCaseCfg = {
  jpeg_in   : "/DEngine/data/1080p/1080p.jpg",
  file_out  : "./jpeg_dec_out.yuv",
  width     : 1920,
  height    : 1080,
  format    : DE_PIX_FMT_MJPEG,
}; 

//发送node，透传数据到设备node
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
	if (fmt != dePixelFormat::DE_PIX_FMT_MJPEG){
    shape.push_back(real_height);			
    shape.push_back(real_width);
    addition.shape_indicator = deShapeCode::HW;
  } else {
    FILE* fp = fopen(pic_path, "rb");
    if (fp == NULL){
      de::NDArray empty;
      return empty;
    }
    fseek(fp, 0L, SEEK_END);
    size_t fileSize = ftell(fp);
    shape.push_back(fileSize);			
    addition.shape_indicator = deShapeCode::W;
  }
	
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
  signal(SIGSEGV, trace);
  signal(SIGABRT, trace);

  de::InitLogLevel(0);

  //创建graph
  de::Graph graph("jpeg_dec");

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
  graph.CreateHostNode("Sender", "jpeg_in");
  graph.CreateHostNode("Receiver", "yuv_out");
  graph.CreateDevNode("de::JpegDecoder", "jpeg_dec");

  //设置graph内node连接，jpeg_in的pouts_[0]连接到jpeg_dec的Pin，使用bridge通道0
  graph.LinkNode("jpeg_in", 0, "jpeg_dec", 0);
  //设置graph内node连接，jpeg_dec的pouts_[0]连接yuv_out的Pin，使用bridage通道1
  graph.LinkNode("jpeg_dec", 0, "yuv_out", 1);

  //设置graph输出，graph的pouts_[0]为yuv_out的pouts_[0]
  graph.SetOutputNode(0, "yuv_out", 0);
  //设置graph输入，graph的Pin[0]为jpeg_in的Pin
  graph.SetInputNode(0, "jpeg_in");

  //启动业务
  graph.Start();

  //启动结果读取线程，获取graph执行结果
  std::thread([](de::Graph *graph) {
    void *p;
    while (nullptr != (p = graph->GetOutput(0))) {
      de::MediaTask *task = static_cast<de::MediaTask*>(p);
      LOG(INFO) << "jpeg ====> yuv " << " decode finished.";
      FILE *file = fopen(kCaseCfg.file_out.c_str(), "w");
      fwrite((char*)task->array.GetTensorData(), task->array.GetTensorDataSize(), 1, file);
	  fclose(file);
      delete task;
    }
  }, &graph).detach();

  //输入jpeg数据
  de::NDArray image = get_image(kCaseCfg.format, kCaseCfg.width, kCaseCfg.height,  kCaseCfg.jpeg_in.c_str(), nullptr);
  for (int i = 0; i < 1; i++) {
      de::MediaTask* p = new de::MediaTask;
      p->array = image;
      p->streamid = 1234;
      p->pts = 5555;
	  p->flag = 0;
	  printf("MediaTask flag %d\n",p->flag);
	  
      graph.SetInput(0, p);
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));
  //确保graph的output队列退出
  graph.StopOutput<de::MediaTask>(0);

  return 0;
}
