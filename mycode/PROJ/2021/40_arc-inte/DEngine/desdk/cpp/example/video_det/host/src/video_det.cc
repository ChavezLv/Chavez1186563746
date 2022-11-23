#include <iostream>
#include <thread>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "de_node.h"
#include "de_model_api.h"
#include "de_graph_api.h"
#include "de_video_in_node.h"
#include "de_video_decoder_node.h"
#include "de_pic_encoder_node.h"
#include "de_pic_decoder_node.h"
#include "de_arctern_node.h"
#include "toolbox.h"


using namespace de;
using namespace de::mm; 


//接收node，从设备node透传数据
class GatherTest : de::Thread {
  public:
  GatherTest(){
    pin.SetTypeInfo(0, "de::arctern::MultiDetRslt", de::TaskDeSerialize<de::arctern::MultiDetRslt>, de::TaskDeleter<de::arctern::MultiDetRslt>);
    pin.SetTypeInfo(1, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pin.SetTypeInfo(2, "de::arctern::MultiDetRsltImg", de::TaskDeSerialize<de::arctern::MultiDetRsltImg>, de::TaskDeleter<de::arctern::MultiDetRsltImg>);

    name_ = "GatherTest";
  }

  void save_pic_json(int id, std::vector<de::arctern::MultiCatDetection> &rects, de::NDArray &img) {
    int num = rects.size();
    std::string json_str = "{[\n";
    for (int i = 0; i < num; i++) {
      std::ostringstream ostr;
      if (i == num - 1) {
        ostr << "    {\"x\":" << rects[i].box.x << ",\"y\":" << rects[i].box.y << ",\"width\":"   << rects[i].box.w << ",\"heigth\":" << rects[i].box.h << ",\"confidence\":"<< rects[i].confidence << ",\"type\":"<<rects[i].type << "}\n";
      }
      else {
        ostr << "    {\"x\":" << rects[i].box.x << ",\"y\":" << rects[i].box.y << ",\"width\":"   << rects[i].box.w << ",\"heigth\":" << rects[i].box.h << ",\"confidence\":"<< rects[i].confidence << ",\"type\":"<<rects[i].type<< "},\n";
      }
      json_str += ostr.str();
    }

    json_str += "]}";

    const ExtInfo* ext = img.GetExtInfo();
    LOG(ERROR)<<" height:"<<ext->roi_rect.h << " width: "<< ext->roi_rect.w;
    std::string jsonname = "org/";
    jsonname += (std::to_string(id) + "_" + ".json");   
    FILE *pFile1 = fopen(jsonname.c_str(), "wb");
    if(pFile1 != NULL) {
      int ret = fwrite(json_str.c_str(), 1, json_str.size(), pFile1);
      fflush(pFile1);
      fclose(pFile1);
    }

    std::string pic_name = "org/";
    pic_name += (std::to_string(id) + "_" + ".jpg");
    FILE *pFile2 = fopen(pic_name.c_str(), "wb");
    if(pFile2 != NULL) {
      int ret = fwrite(img.GetTensorData(), 1, img.GetTensorDataSize(), pFile2);
      fflush(pFile2);
      fclose(pFile2);
    }

  }

  void Stop(void) {}
  
  void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) {
    switch (task_type)
    {
      case 0:
      {
        de::arctern::MultiDetRslt* pTask = static_cast<de::arctern::MultiDetRslt*>(rx_task);
        LOG(ERROR)<<"rcv MultiDetRslt,stream_id:"<<pTask->stream_id<<",id:"<<pTask->id << ", rects: "<< pTask->rects.size();
        if (Imgs.count(pTask->id)==1) {
          /* 叠框 */
          de::NDArray img = Imgs[pTask->id];
          de::PicDraw draw(img);
          std::vector<std::string> labels;
          for (auto rect : pTask->rects){
            std::vector<std::string> labels;
            std::stringstream label_str;
            label_str << "conf:" << rect.confidence << " type:" << rect.type;
            //LOG(ERROR) << "label_str" << label_str.str();
            labels.emplace_back(label_str.str());
            draw.AddRectText(rect.box, labels);
          }
          save_pic_json(pTask->id, pTask->rects, img);
          /*  存图*/
          std::stringstream s_id;
          s_id << pTask->id;
          std::string id;
          s_id >> id;
          std::string path = "img/";
          path += (id+"_"+".jpg");   
          draw.Save(path);
          LOG(ERROR) << "[video det]: save pic" << path;

          Imgs.erase(pTask->id);
        }
        else {
          if (pTask->rects.size() > 0) Rois[pTask->id] = pTask->rects;
        }
      }
      break;
      case 1:
      {
        de::MediaTask* pTask = static_cast<de::MediaTask*>(rx_task);
        LOG(ERROR) << "rcv image: frame id " <<  pTask->pts;

        if (Rois.count(pTask->pts)==1){
          /* 叠框 */
          de::NDArray img = pTask->array;
          de::PicDraw draw(img);
          for (auto rect : Rois[pTask->pts]) {
            std::vector<std::string> labels;
            std::stringstream label_str;
            label_str << "conf:" << rect.confidence;
            labels.emplace_back(label_str.str());
            label_str.clear();
            label_str.str("");
            label_str << "type:" << rect.type;
            labels.emplace_back(label_str.str());
            draw.AddRectText(rect.box, labels);
          }
          save_pic_json(pTask->pts, Rois[pTask->pts], img);
          /*  存图*/
          std::stringstream s_id;
          s_id << pTask->pts;
          std::string id;
          s_id >> id;
          std::string path = "img/";
          path += (id+"_"+".jpg");   
          draw.Save(path);
          index++;
          LOG(ERROR) << "[video det]: save pic" << path;

          Rois.erase(pTask->frameid);
        } else {
            Imgs[pTask->frameid] = pTask->array;
        }
      }
      break;
      case 2:
      {
        de::arctern::MultiDetRsltImg* pTask = static_cast<de::arctern::MultiDetRsltImg*>(rx_task);
        LOG(ERROR) << "rcv image: frame id " <<  pTask->id;
        /* 叠框 */
        de::NDArray img = pTask->img;
        de::PicDraw draw(img);

        for (auto rect : pTask->rects){
          std::vector<std::string> labels;
          std::stringstream label_str;
          //label_str << "conf:" << rect.confidence << " type:" << rect.type;
          label_str << "conf:" << rect.confidence;
          //LOG(ERROR) << label_str.str();
          labels.emplace_back(label_str.str());
          label_str.clear();
          label_str.str("");
          label_str << "type:" << rect.type;
          //LOG(ERROR) << label_str.str();
          draw.AddRectText(rect.box, labels);
        }
        save_pic_json(pTask->id, pTask->rects, img);
        /*  存图*/
        std::stringstream s_id;
        s_id << pTask->id;
        std::string id;
        s_id >> id;
        std::string path = "img/";
        path += (id+"_"+".jpg");   
        draw.Save(path);
        index++;
        LOG(ERROR) << "[video det]: save pic" << path;
      }
      break;
      default:
        LOG(ERROR) << "type not recoginzed!";
        break;
    }

    pin.DelTask(rx_task, task_type);
  }

  uint32_t index=0;
  std::map<uint32_t, de::NDArray> Imgs;
  std::map<uint32_t, std::vector<de::arctern::MultiCatDetection>>  Rois;

};

DE_CLASS_REGISTER("GatherTest", GatherTest);


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
void trace(int signo) {
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


int main(int argc, char* argv[]) {
  #ifdef DEBUG_ABORT_TRACE_ON
  signal(SIGSEGV, trace);
  signal(SIGABRT, trace);
  #endif

  void* handle = de::Device::LoadLib(0,"host:/DEngine/desdk/cpp/example/nodebase/dev/lib/libdevnodes.so");
  
  std::string model_net_path;
  std::string model_param_path;

  model_net_path = "host:/DEngine/model/dp1000/caffe_yolov3_416/net.bin";
  model_param_path = "host:/DEngine/model/dp1000/caffe_yolov3_416/model.bin";
  
  //加载模型到芯片内存
  de::ModelManager mem(0);
  mem.Load(model_net_path, model_param_path, false);
  
  //创建graph
  de::Graph graph("demo");
  graph.CreateBridge(0);
  graph.AddBridgeH2DChan(0, "Host2Device");
  graph.AddBridgeD2HChan(1, "Device2Host");
  graph.AddBridgeD2HChan(2, "Device2Host");
  graph.AddBridgeD2HChan(3, "Device2Host");
  
  //创建node
  graph.CreateHostNode("de::VideoInput", "video_in");
  graph.CreateDevNode("de::VideoDecoder", "video_dec");

  graph.CreateDevNode("de::arctern::TestVideoFrameFilter", "TestVideoFrameFilter");        
  graph.CreateDevNode("de::arctern::ObjDet", "ObjDetNode");
  graph.CreateDevNode("de::arctern::ObjDet", "ObjDetNode1");
  graph.CreateDevNode("de::arctern::ObjDet", "ObjDetNode2");
  graph.CreateDevNode("de::arctern::ObjDet", "ObjDetNode3");
  graph.CreateDevNode("de::arctern::ObjDet", "ObjDetNode4");
  graph.CreateDevNode("de::arctern::ObjDet", "ObjDetNode5");

  graph.CreateDevNode("de::arctern::ObjDetRsltDispatcher", "DetRsltDispatcherNode");
  graph.CreateHostNode("GatherTest", "GatherTestNode");
  graph.CreateDevNode("de::JpegEncoder", "JpegEncoderNode");

  //设置node属性
  graph.SetNodeAttr("ObjDetNode", "model_net_path", model_net_path);
  graph.SetNodeAttr("ObjDetNode", "model_param_path", model_param_path);
  graph.SetNodeAttr("ObjDetNode", "resize_type", 1);
  graph.SetNodeAttr("ObjDetNode", "batch_num", 1);
  graph.SetNodeAttr("ObjDetNode", "resource_num", 1);
  graph.SetNodeAttr("ObjDetNode", "profile.enable", 1);

  graph.SetNodeAttr("ObjDetNode1", "model_net_path", model_net_path);
  graph.SetNodeAttr("ObjDetNode1", "model_param_path", model_param_path);
  graph.SetNodeAttr("ObjDetNode1", "resize_type", 1);
  graph.SetNodeAttr("ObjDetNode1", "batch_num", 1);
  graph.SetNodeAttr("ObjDetNode1", "resource_num", 1);
  graph.SetNodeAttr("ObjDetNode1", "profile.enable", 1);

  graph.SetNodeAttr("ObjDetNode2", "model_net_path", model_net_path);
  graph.SetNodeAttr("ObjDetNode2", "model_param_path", model_param_path);
  graph.SetNodeAttr("ObjDetNode2", "resize_type", 1);
  graph.SetNodeAttr("ObjDetNode2", "batch_num", 1);
  graph.SetNodeAttr("ObjDetNode2", "resource_num", 1);
  graph.SetNodeAttr("ObjDetNode2", "profile.enable", 1);

  graph.SetNodeAttr("ObjDetNode3", "model_net_path", model_net_path);
  graph.SetNodeAttr("ObjDetNode3", "model_param_path", model_param_path);
  graph.SetNodeAttr("ObjDetNode3", "resize_type", 1);
  graph.SetNodeAttr("ObjDetNode3", "batch_num", 1);
  graph.SetNodeAttr("ObjDetNode3", "resource_num", 1);
  graph.SetNodeAttr("ObjDetNode3", "profile.enable", 1);

  graph.SetNodeAttr("ObjDetNode4", "model_net_path", model_net_path);
  graph.SetNodeAttr("ObjDetNode4", "model_param_path", model_param_path);
  graph.SetNodeAttr("ObjDetNode4", "resize_type", 1);
  graph.SetNodeAttr("ObjDetNode4", "batch_num", 1);
  graph.SetNodeAttr("ObjDetNode4", "resource_num", 1);
  graph.SetNodeAttr("ObjDetNode4", "profile.enable", 1);

  graph.SetNodeAttr("ObjDetNode5", "model_net_path", model_net_path);
  graph.SetNodeAttr("ObjDetNode5", "model_param_path", model_param_path);
  graph.SetNodeAttr("ObjDetNode5", "resize_type", 1);
  graph.SetNodeAttr("ObjDetNode5", "batch_num", 1);
  graph.SetNodeAttr("ObjDetNode5", "resource_num", 1);
  graph.SetNodeAttr("ObjDetNode5", "profile.enable", 1);   

  graph.SetNodeAttr("TestVideoFrameFilter", "filter.period", 5); 
  graph.SetNodeAttr("TestVideoFrameFilter", "parallel_num", 3);

  //设置node属性
  char *uri = argv[1];
  graph.SetNodeAttr("video_in", "uri", uri);
  graph.SetNodeAttr("video_in", "stream-id", 1);
  int width = (int)atoi(argv[2]);
  int height = (int)atoi(argv[3]);    
  printf("uri: %s, width: %d, height: %d\n", uri, width, height);
  graph.SetNodeAttr("video_in", "width", width);
  graph.SetNodeAttr("video_in", "height", height);
  graph.SetNodeAttr("video_in", "framerate", 25.0);
  graph.SetNodeAttr("video_in", "lower-trans-type", 0);
  graph.SetNodeAttr("video_in", "stream-type", DE_PIX_FMT_H264);

  graph.SetNodeAttr("video_dec", "output-buffers", 16);
  graph.SetNodeAttr("video_dec", "stream-type", DE_PIX_FMT_H264);
  graph.SetNodeAttr("video_dec", "pin.que.size", 16);

  graph.LinkNode("video_in", 0, "video_dec", 0);
  graph.LinkNode("video_dec", 0, "TestVideoFrameFilter");

  graph.LinkNode("TestVideoFrameFilter", 0, "ObjDetNode");
  graph.LinkNode("TestVideoFrameFilter", 1, "ObjDetNode1");
  graph.LinkNode("TestVideoFrameFilter", 2, "ObjDetNode2");  
  graph.LinkNode("TestVideoFrameFilter", 3, "ObjDetNode3");
  graph.LinkNode("TestVideoFrameFilter", 4, "ObjDetNode4");
  graph.LinkNode("TestVideoFrameFilter", 5, "ObjDetNode5");

  graph.LinkNode("ObjDetNode", 0, "DetRsltDispatcherNode");
  graph.LinkNode("ObjDetNode1", 0, "DetRsltDispatcherNode");
  graph.LinkNode("ObjDetNode2", 0, "DetRsltDispatcherNode");
  graph.LinkNode("ObjDetNode3", 0, "DetRsltDispatcherNode");
  graph.LinkNode("ObjDetNode4", 0, "DetRsltDispatcherNode");
  graph.LinkNode("ObjDetNode5", 0, "DetRsltDispatcherNode");

  graph.LinkNode("DetRsltDispatcherNode", 0, "GatherTestNode", 1);
  graph.LinkNode("DetRsltDispatcherNode", 1, "JpegEncoderNode");

  graph.LinkNode("JpegEncoderNode", 0, "GatherTestNode", 2);

  //启动业务
  graph.Start();

  while(1)
  {
    std::this_thread::sleep_for(std::chrono::seconds(300));
  }

  de::Device::UnLoadLibHandle(0,handle);
}
