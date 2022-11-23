#include <iostream>
#include <thread>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "de_node.h"
#include <de_model_api.h>
#include "de_graph_api.h"
#include "de_video_in_node.h"
#include "de_video_decoder_node.h"
#include "de_pic_encoder_node.h"
#include "de_arctern_node.h"
#include "toolbox.h"
#include "de_pic_decoder_node.h"

using namespace de;
using namespace de::mm; 

int flag = 0;

class PicSenderNode : de::Thread {
  public:
  PicSenderNode(){
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
    attr_.SetDefault("pic_path", "");
    attr_.SetDefault("interval", 500, 0, 60000);
  }
  void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT){}

  void Start(){
    std::string path;
    int interval = 1000;
    attr_.Get("pic_path", path);
    attr_.Get("interval", interval);
    printf("------PicSenderNode, path: %s\n", path.c_str());
    std::thread([this](std::string path, int interval) {
      while (1) {
          
        if (exit_flag_) break;
        
        if (flag == 1){
          std::this_thread::sleep_for(std::chrono::seconds(1));
          continue;
        }

        if (!path.empty()){ 
          DIR *d = NULL;
          d = opendir(path.c_str());
          if (!d){
            printf("%s is error\n", path.c_str());
            break;
          }
          struct dirent *ptr = NULL;
          int pts = 1;
          int stream_id = 1;
          char file_name[1024]={0};
          while ((ptr = readdir(d)) != NULL) {
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0)){
              continue;
            }
            
            if (ptr->d_type == DT_DIR){
              continue;
            }
            
            if (ptr->d_type == DT_REG){
              sprintf(file_name,"%s/%s",path.c_str(), ptr->d_name);
              printf("file_name: %s\n", file_name);
              cv::Mat cv_img = cv::imread(file_name);
              if (cv_img.empty()){
                continue;
              }

              cv::Size dsize = cv::Size((cv_img.cols/16)*16, (cv_img.rows/2)*2);
              cv::Mat img2 = cv::Mat(dsize, CV_32S);
              cv::resize(cv_img, img2, dsize);
              
              cv::Mat cv_yuv;
              cv::cvtColor(img2, cv_yuv, CV_BGR2YUV_I420);
  
              de::MediaTask* tx_task = new de::MediaTask();
              std::vector<int64_t> shape;
              DLDataType dtype = {kDLInt, 8, 1};
              ExtInfo info = {DE_PIX_FMT_NV12, deShapeCode::HW, {0, 0, img2.cols, img2.rows}};
              
              shape.push_back(img2.rows * 3 / 2);
              shape.push_back(img2.cols);
              info.shape_indicator = deShapeCode::HW ;
              dtype.lanes = 1;
              tx_task->array = de::NDArray::Empty(shape, dtype, { kDLCPU, 0 }, info);
              int len = tx_task->array.GetTensorDataSize();
              
              uchar *buf = new uchar[img2.rows * img2.cols / 2];
              memcpy(buf, cv_yuv.data + img2.rows * img2.cols, img2.rows * img2.cols / 2);
              for(int32_t i = 0; i < img2.rows * img2.cols / 4; i++){
                cv_yuv.data[img2.rows * img2.cols + 2 * i] = buf[i];
                cv_yuv.data[img2.rows * img2.cols + 2 * i + 1] = buf[img2.rows * img2.cols / 4 + i];
              }
              delete buf;

              memcpy(tx_task->array.GetTensorData(), cv_yuv.data, len);
              
              printf("----len: %d", len);

              tx_task->pts = pts++;
              tx_task->streamid = stream_id;
              tx_task->frameid = pts++;
              pouts_[0]->SendTask(tx_task);
              memset(file_name, 0, 1024);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
          }
          
          closedir(d);
          std::this_thread::sleep_for(std::chrono::seconds(100));
          break;
        }
        break;
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
      }
    }, path, interval).detach();
  }
  void Stop(void){pin.DestroyQueue();exit_flag_=true;}
  private:
  bool exit_flag_=false;
};

DE_CLASS_REGISTER("PicSenderNode", PicSenderNode);

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
          LOG(ERROR) << "[pic det]: save pic" << path;

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
          LOG(ERROR) << "[pic det]: save pic" << path;

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
        LOG(ERROR) << "[pic det]: save pic" << path;
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




#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
bool gExitFlag = false;
void signal_exit_handler(int sig)
{
#if !(defined(_WIN32) && !defined(_WIN64))
  if (sig == SIGTERM)
  {
      printf("receive SIGTERM signal, ready to exit\n");
  }
  else if(sig == SIGINT)
  {
      printf("receive SIGINT signal, ready to exit\n");
  }

  gExitFlag = true;
#endif

    //exit(0);
}


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



int main(int argc, char* argv[]) {
  signal(SIGTERM, signal_exit_handler);
  signal(SIGINT, signal_exit_handler);
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

  char *pic_path = NULL;
  if (strstr(argv[1], "jpg://")){
    pic_path = argv[1] + 6;
    printf("jpg load, pic dir: %s \n", pic_path);
  }

  //创建graph
  de::Graph graph("demo");
  graph.CreateBridge(0);
  graph.AddBridgeH2DChan(0, "Host2Device");
  graph.AddBridgeD2HChan(1, "Device2Host");
  graph.AddBridgeD2HChan(2, "Device2Host");
  graph.AddBridgeD2HChan(3, "Device2Host");
  //创建node
  graph.CreateHostNode("PicSenderNode", "PicSender");
  
  graph.CreateDevNode("de::arctern::TestPicDecFrameFilter", "TestPicDecFrameFilter");
  graph.CreateDevNode("de::arctern::PicObjDet", "PicObjDetNode");
  graph.CreateDevNode("de::arctern::PicObjDet", "PicObjDetNode1");
  graph.CreateDevNode("de::arctern::PicObjDet", "PicObjDetNode2");
  graph.CreateDevNode("de::arctern::PicObjDet", "PicObjDetNode3");
  graph.CreateDevNode("de::arctern::PicObjDet", "PicObjDetNode4");
  graph.CreateDevNode("de::arctern::PicObjDet", "PicObjDetNode5");


  graph.CreateDevNode("de::arctern::ObjDetRsltDispatcher", "DetRsltDispatcherNode");
  graph.CreateHostNode("GatherTest", "GatherTestNode");
  graph.CreateDevNode("de::JpegEncoder", "JpegEncoderNode");

  //设置node属性
  graph.SetNodeAttr("PicObjDetNode", "model_net_path", model_net_path);
  graph.SetNodeAttr("PicObjDetNode", "model_param_path", model_param_path);
  graph.SetNodeAttr("PicObjDetNode", "resize_type", 1);
  graph.SetNodeAttr("PicObjDetNode", "batch_num", 1);
  graph.SetNodeAttr("PicObjDetNode", "resource_num", 1);
  graph.SetNodeAttr("PicObjDetNode", "profile.enable", 1);

  graph.SetNodeAttr("TestPicDecFrameFilter", "parallel_num", 3);
  
  graph.SetNodeAttr("PicObjDetNode1", "model_net_path", model_net_path);
  graph.SetNodeAttr("PicObjDetNode1", "model_param_path", model_param_path);
  graph.SetNodeAttr("PicObjDetNode1", "resize_type", 1);
  graph.SetNodeAttr("PicObjDetNode1", "batch_num", 1);
  graph.SetNodeAttr("PicObjDetNode1", "resource_num", 1);
  graph.SetNodeAttr("PicObjDetNode1", "profile.enable", 1);

  graph.SetNodeAttr("PicObjDetNode2", "model_net_path", model_net_path);
  graph.SetNodeAttr("PicObjDetNode2", "model_param_path", model_param_path);
  graph.SetNodeAttr("PicObjDetNode2", "resize_type", 1);
  graph.SetNodeAttr("PicObjDetNode2", "batch_num", 1);
  graph.SetNodeAttr("PicObjDetNode2", "resource_num", 1);
  graph.SetNodeAttr("PicObjDetNode2", "profile.enable", 1);

  graph.SetNodeAttr("PicObjDetNode3", "model_net_path", model_net_path);
  graph.SetNodeAttr("PicObjDetNode3", "model_param_path", model_param_path);
  graph.SetNodeAttr("PicObjDetNode3", "resize_type", 1);
  graph.SetNodeAttr("PicObjDetNode3", "batch_num", 1);
  graph.SetNodeAttr("PicObjDetNode3", "resource_num", 1);
  graph.SetNodeAttr("PicObjDetNode3", "profile.enable", 1);

  graph.SetNodeAttr("PicObjDetNode4", "model_net_path", model_net_path);
  graph.SetNodeAttr("PicObjDetNode4", "model_param_path", model_param_path);
  graph.SetNodeAttr("PicObjDetNode4", "resize_type", 1);
  graph.SetNodeAttr("PicObjDetNode4", "batch_num", 1);
  graph.SetNodeAttr("PicObjDetNode4", "resource_num", 1);
  graph.SetNodeAttr("PicObjDetNode4", "profile.enable", 1);

  graph.SetNodeAttr("PicObjDetNode5", "model_net_path", model_net_path);
  graph.SetNodeAttr("PicObjDetNode5", "model_param_path", model_param_path);
  graph.SetNodeAttr("PicObjDetNode5", "resize_type", 1);
  graph.SetNodeAttr("PicObjDetNode5", "batch_num", 1);
  graph.SetNodeAttr("PicObjDetNode5", "resource_num", 1);
  graph.SetNodeAttr("PicObjDetNode5", "profile.enable", 1);

  //设置node属性
  graph.SetNodeAttr("PicSender", "pic_path", pic_path);
  
  graph.LinkNode("PicSender", 0, "TestPicDecFrameFilter", 0);

  graph.LinkNode("TestPicDecFrameFilter", 0, "PicObjDetNode");
  graph.LinkNode("TestPicDecFrameFilter", 1, "PicObjDetNode1");
  graph.LinkNode("TestPicDecFrameFilter", 2, "PicObjDetNode2");  
  graph.LinkNode("TestPicDecFrameFilter", 3, "PicObjDetNode3");
  graph.LinkNode("TestPicDecFrameFilter", 4, "PicObjDetNode4");
  graph.LinkNode("TestPicDecFrameFilter", 5, "PicObjDetNode5");

  graph.LinkNode("PicObjDetNode", 0, "DetRsltDispatcherNode");
  graph.LinkNode("PicObjDetNode1", 0, "DetRsltDispatcherNode");
  graph.LinkNode("PicObjDetNode2", 0, "DetRsltDispatcherNode");
  graph.LinkNode("PicObjDetNode3", 0, "DetRsltDispatcherNode");
  graph.LinkNode("PicObjDetNode4", 0, "DetRsltDispatcherNode");
  graph.LinkNode("PicObjDetNode5", 0, "DetRsltDispatcherNode");

  graph.LinkNode("DetRsltDispatcherNode", 0, "GatherTestNode", 1);
  graph.LinkNode("DetRsltDispatcherNode", 1, "JpegEncoderNode");

  graph.LinkNode("JpegEncoderNode", 0, "GatherTestNode", 2);

  //启动业务
  graph.Start();

  while(1){
    std::this_thread::sleep_for(std::chrono::seconds(300));
  }

  de::Device::UnLoadLibHandle(0,handle);
}
