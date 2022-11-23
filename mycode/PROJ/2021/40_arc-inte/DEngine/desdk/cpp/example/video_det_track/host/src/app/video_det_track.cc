#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include "de_node.h"
#include "de_graph_api.h"
#include "de_model_api.h"
#include "de_node_common.h"
#include "de_aiengine_ext_node.h"
#include "de_video_in_node.h"
#include "de_file_in_node.h"
#include "de_video_decoder_node.h"
#include "de_task_serialize.h"
#include "ws_c_api.h"
#include "media_session.h"
#include "de_arctern_node.h"


using namespace intellif;
using namespace de;

if_log_level_t g_sdk_log_level = IF_LOG_LEVEL_DEBUG;
if_sdk_log_cb_t g_sdk_log_cb;
//streamid - disp_obj
std::map<int, intellif::MediaDisplay*> stream_disp_map;
//streamid - sessionid
std::map<int, intellif::MediaSession*> intellif::media_session_map;

void onopen(int id, void* usrdata){
  printf("client media session id %d.\n", id);  media_session_map[id] = new MediaSession(id,E_CLIENT);
}

void onclose(int id, void* usrdata){
  printf("close client media session id %d.\n", id);  media_session_map.erase(id);
}

void onmsg(int id, const void* msg, int len,  void* usrdata){}

namespace de {
/// @brief 接收者node，用于接收接收RTSP流
class StreamReceiver : public de::Thread {
  public:
  StreamReceiver() {
    pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
  }
  ~StreamReceiver() {
    pouts_[0]->DestroyQueue<de::MediaTask>();
    //等待队列线程退出
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  void Proc(void* rx_task, int32_t task_type,POutType pout_type) {
    
    if (task_type == 0) {
      pouts_[0]->SendTask(rx_task);
    }
    else {
      pin.DelTask(rx_task, task_type);
    }
  }

  void Stop(void){
    pin.DestroyQueue();
  }
};

/// @brief 接收者node，用于接收芯片侧结果或主控接收rtsp流
class NnReceiver : public de::Thread {
  public:
  NnReceiver() {
    pin.SetTypeInfo(0, "de::arctern::MultiTrackResult", de::TaskDeSerialize<de::arctern::MultiTrackResult>, de::TaskDeleter<de::arctern::MultiTrackResult>);
    pouts_[0]->SetTypeInfo("de::arctern::MultiTrackResult", de::TaskSerialize<de::arctern::MultiTrackResult>);
  }
  ~NnReceiver() {
    pouts_[0]->DestroyQueue<de::arctern::MultiTrackResult>();
  }

  void Proc(void* rx_task, int32_t task_type,POutType pout_type) {
    if (task_type == 0 && pouts_[0]->IsLinked()) {
      pouts_[0]->SendTask(rx_task);
    }
    else {
      pin.DelTask(rx_task, task_type);
    }
  }

  void Stop(void) {
    pin.DestroyQueue();
  }
};

class VideoInput2 : public de::Thread {
  public:
  VideoInput2() {
    pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);     
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<MediaTask>);
    pouts_[1]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<MediaTask>);        
  }

  void Proc(void* rx_task, int32_t task_type,POutType pout_type) {
    de::MediaTask* p = (de::MediaTask*)rx_task;
    assert(p->array.defined());
    timespec time1;
    clock_gettime(CLOCK_MONOTONIC,&time1);//CLOCK_REALTIME CLOCK_MONOTONIC
    uint64_t curtime = time1.tv_sec*1000000000 + time1.tv_nsec;
    p->pts = curtime/1000000;
    ExtInfo* info = const_cast<ExtInfo*>(p->array.GetExtInfo());
    info->pts = p->pts;
    p->flag = 0;
    de::MediaTask* tx_task = new de::MediaTask;
    tx_task->streamid = p->streamid;
    tx_task->frameid = p->frameid;
    tx_task->flag = p->flag;
    tx_task->pts = p->pts;
    tx_task->array = p->array;
    
    if (task_type == 0 && pouts_[0]->IsLinked())
      pouts_[0]->SendTask(rx_task);
    if (task_type == 0 && pouts_[1]->IsLinked()){
      assert(tx_task->array.defined());
      pouts_[1]->SendTask(tx_task);
    }
  }

  void Stop(void){
    pin.DestroyQueue();
  }
  ~VideoInput2() {
    pouts_[0]->DestroyQueue<de::MediaTask>();
    pouts_[1]->DestroyQueue<de::MediaTask>();
  }
};


//注册全局node
DE_CLASS_REGISTER("de::StreamReceiver", de::StreamReceiver);
DE_CLASS_REGISTER("de::NnReceiver", de::NnReceiver);
DE_CLASS_REGISTER("de::VideoInput2", de::VideoInput2);



de::Graph * CreateGraph(int32_t dev_id, int32_t stream_id, const std::string& cfg_file) {
  LOG(INFO) << "Start Graph! config json file =" << cfg_file;
  // modify node cfg attr
  de::GraphInfo *info = new de::GraphInfo();
  info->FromFile(cfg_file);
  de::NodeAttrInfo attr_info ;
  attr_info.attr_name = "stream-id";
  attr_info.attr_type = "int";
  attr_info.attr_value.ivalue = stream_id;
  info->SetNodeAttr("VideoInputNode", attr_info);

  info->AddNodeAttr("GatherNode", "dev.id", dev_id);
  info->AddNodeAttr("GatherNode", "stream.id", stream_id);

  //modify bridge dev_id
  info->bridges[0].dev_id = dev_id;

  de::Graph *graph = new de::Graph(*info);
  delete info;

  return graph;
}

class VideoPredict {
  public:
    VideoPredict(std::string graph_name, int dev_id, int stream_id, std::string uri, std::string model_path, int ws_port, int stream_type, int stream_width, int stream_height, int stream_fps, bool encrypt,int display_mode) : 
        dev_id_(dev_id), stream_id_(stream_id), uri_(uri), model_path_(model_path), ws_port_(ws_port), local_stream_type_(stream_type)
    {
      LoadModel(encrypt);
      if (display_mode == 1) remote_disp_= false;
	
      if (uri_.substr(uri.size()-5) == ".json"){
        graph_ = CreateGraph(dev_id_,stream_id_,uri);
      }
      else
      {
        bool uriisfile = false;
        if (uri_.substr(0,5) == "file:")
          uriisfile = true;
        {
          graph_ = new de::Graph(graph_name);
          //创建bridge，连接取流和解码2个node
          graph_->CreateBridge(dev_id_);
          std::map<std::string, int> attr;
          attr["block_max_size"] = 768*1024;
          attr["need_check_crc"] = 0;
          attr["queue_size"] = 16;
          graph_->AddBridgeH2DChan(0, std::string("Host2DevVideoStreamChannel") + std::string("_") + std::to_string(stream_id_), attr);
          attr["block_max_size"] = 512*1024;
          graph_->AddBridgeD2HChan(1, "Dev2HostDataChan", attr);

          //创建node
                if (uriisfile)
                  graph_->CreateHostNode("de::FileInput", "VideoInputNode");
                else
            graph_->CreateHostNode("de::VideoInput", "VideoInputNode");
          graph_->CreateHostNode("de::VideoInput2", "VideoInputNode2");
          if (local_stream_type_ == DE_PIX_FMT_H264)
            graph_->CreateDevNode("de::VideoDecoder", "VideoDecoderNode");
          else if (local_stream_type_ == DE_PIX_FMT_MJPEG){
            graph_->CreateDevNode("de::JpegDecoder", "VideoDecoderNode");     
            graph_->CreateDevNode("de::arctern::JpegDecRsltHandle", "JpegDecRsltHandle"); 
              }
          //det node
          graph_->CreateDevNode("de::arctern::TestVideoFrameFilter", "TestVideoFrameFilter"); 
          if (encrypt) //only for dearctern model
                  graph_->CreateDevNode("de::arctern::MultiCatDet", "MultiCatDetNode1");
                else 
                  graph_->CreateDevNode("de::arctern::ObjDet", "MultiCatDetNode1");
          graph_->CreateDevNode("de::arctern::ObjDet", "MultiCatDetNode2");
          graph_->CreateDevNode("de::arctern::ObjDet", "MultiCatDetNode3");
          graph_->CreateDevNode("de::arctern::ObjDet", "MultiCatDetNode4");
          graph_->CreateDevNode("de::arctern::ObjDet", "MultiCatDetNode5");
          graph_->CreateDevNode("de::arctern::ObjDet", "MultiCatDetNode6");
                
          // track_sync
          graph_->CreateDevNode("de::arctern::TrackSyncNode", "TrackSyncNode");

          graph_->CreateHostNode("de::StreamReceiver", "StreamReceiver");
          graph_->CreateHostNode("de::NnReceiver", "NnReceiver");
        }
        
        {
          graph_->SetGraphAttr("hwlink.sched.rate.enable",1);
          //设置node属性
          if (uriisfile) {
            graph_->SetNodeAttr("VideoInputNode", "file", uri_);
            graph_->SetNodeAttr("VideoInputNode", "loop", -1);
          }
          else {
            graph_->SetNodeAttr("VideoInputNode", "uri", uri_);
            graph_->SetNodeAttr("VideoInputNode", "lower-trans-type", 0);
          }
          graph_->SetNodeAttr("VideoInputNode", "stream-id", stream_id_);
          graph_->SetNodeAttr("VideoInputNode", "width", stream_width);
          graph_->SetNodeAttr("VideoInputNode", "height", stream_height);
          graph_->SetNodeAttr("VideoInputNode", "framerate", (float)stream_fps);
          
          if (local_stream_type_ == DE_PIX_FMT_MJPEG){
            graph_->SetNodeAttr("JpegDecRsltHandle", "width", stream_width);
            graph_->SetNodeAttr("JpegDecRsltHandle", "height", stream_height);
          }
          
          graph_->SetNodeAttr("VideoInputNode", "stream-type", local_stream_type_);

          if (local_stream_type_ == DE_PIX_FMT_H264){
            graph_->SetNodeAttr("VideoDecoderNode", "pin.que.size", 16);
            graph_->SetNodeAttr("VideoDecoderNode", "output-buffers", 32);
            graph_->SetNodeAttr("VideoDecoderNode", "stream-type", DE_PIX_FMT_H264);
          }
           
          graph_->SetNodeAttr("TestVideoFrameFilter", "filter.period", 8); 
          graph_->SetNodeAttr("TestVideoFrameFilter", "parallel_num", 4);

          if (encrypt){
            graph_->SetNodeAttr("TestVideoFrameFilter", "filter.period", 1); 
            graph_->SetNodeAttr("TestVideoFrameFilter", "parallel_num", 1);
          }

          // set MultiDectNode attr
          graph_->SetNodeAttr("MultiCatDetNode1", "encrypt", encrypt);
          graph_->SetNodeAttr("MultiCatDetNode1", "model_net_path", model_path_ + "/net.bin");    
          graph_->SetNodeAttr("MultiCatDetNode1", "model_param_path", model_path_ + "/model.bin");
          graph_->SetNodeAttr("MultiCatDetNode2", "model_net_path", model_path_ + "/net.bin");    
          graph_->SetNodeAttr("MultiCatDetNode2", "model_param_path", model_path_ + "/model.bin");
          graph_->SetNodeAttr("MultiCatDetNode3", "model_net_path", model_path_ + "/net.bin");    
          graph_->SetNodeAttr("MultiCatDetNode3", "model_param_path", model_path_ + "/model.bin");
          graph_->SetNodeAttr("MultiCatDetNode4", "model_net_path", model_path_ + "/net.bin");    
          graph_->SetNodeAttr("MultiCatDetNode4", "model_param_path", model_path_ + "/model.bin");
          graph_->SetNodeAttr("MultiCatDetNode5", "model_net_path", model_path_ + "/net.bin");    
          graph_->SetNodeAttr("MultiCatDetNode5", "model_param_path", model_path_ + "/model.bin");
          graph_->SetNodeAttr("MultiCatDetNode6", "model_net_path", model_path_ + "/net.bin");    
          graph_->SetNodeAttr("MultiCatDetNode6", "model_param_path", model_path_ + "/model.bin");
          
          if (encrypt) graph_->SetNodeAttr("MultiCatDetNode1", "period", 8);
          
          graph_->SetNodeAttr("MultiCatDetNode1", "resize_type", 1);
          graph_->SetNodeAttr("MultiCatDetNode1", "batch_num", 1);
          graph_->SetNodeAttr("MultiCatDetNode1", "resource_num", 1);
          // graph_->SetNodeAttr("MultiCatDetNode1", "profile.enable", 1);
          graph_->SetNodeAttr("MultiCatDetNode2", "resize_type", 1);
          graph_->SetNodeAttr("MultiCatDetNode2", "batch_num", 1);
          graph_->SetNodeAttr("MultiCatDetNode2", "resource_num", 1);
          // graph_->SetNodeAttr("MultiCatDetNode2", "profile.enable", 1);
          graph_->SetNodeAttr("MultiCatDetNode3", "resize_type", 1);
          graph_->SetNodeAttr("MultiCatDetNode3", "batch_num", 1);
          graph_->SetNodeAttr("MultiCatDetNode3", "resource_num", 1);
          // graph_->SetNodeAttr("MultiCatDetNode3", "profile.enable", 1);
          graph_->SetNodeAttr("MultiCatDetNode4", "resize_type", 1);
          graph_->SetNodeAttr("MultiCatDetNode4", "batch_num", 1);
          graph_->SetNodeAttr("MultiCatDetNode4", "resource_num", 1);
          // graph_->SetNodeAttr("MultiCatDetNode4", "profile.enable", 1);
          graph_->SetNodeAttr("MultiCatDetNode5", "resize_type", 1);
          graph_->SetNodeAttr("MultiCatDetNode5", "batch_num", 1);
          graph_->SetNodeAttr("MultiCatDetNode5", "resource_num", 1);
          // graph_->SetNodeAttr("MultiCatDetNode5", "profile.enable", 1);
          graph_->SetNodeAttr("MultiCatDetNode6", "resize_type", 1);
          graph_->SetNodeAttr("MultiCatDetNode6", "batch_num", 1);
          graph_->SetNodeAttr("MultiCatDetNode6", "resource_num", 1);
          // graph_->SetNodeAttr("MultiCatDetNode6", "profile.enable", 1);

          //set tracksyncNode attr
          graph_->SetNodeAttr("TrackSyncNode","det_bypass", 0);
          graph_->SetNodeAttr("TrackSyncNode", "sync.period.frm", 8);
          graph_->SetNodeAttr("TrackSyncNode", "debug.level", 1);
          graph_->SetNodeAttr("TrackSyncNode", "track.period.frm", 2);
          graph_->SetNodeAttr("TrackSyncNode", "face.track.enable", 1);
          graph_->SetNodeAttr("TrackSyncNode", "body.track.enable", 1);
          graph_->SetNodeAttr("TrackSyncNode", "car.track.enable", 1);
          graph_->SetNodeAttr("TrackSyncNode", "bike.track.enable", 1);
          graph_->SetNodeAttr("TrackSyncNode", "license.track.enable", 1);
          graph_->SetNodeAttr("TrackSyncNode", "face.qual.batch.num", 20);
          graph_->SetNodeAttr("TrackSyncNode", "human.qual.batch.num", 20);
          graph_->SetNodeAttr("TrackSyncNode", "car.qual.batch.num", 20);
          graph_->SetNodeAttr("TrackSyncNode", "bike.qual.batch.num", 20);
          graph_->SetNodeAttr("TrackSyncNode", "face.track.max.num", 10);
          graph_->SetNodeAttr("TrackSyncNode", "human.track.max.num", 10);
          graph_->SetNodeAttr("TrackSyncNode", "human.rect.shrink.left", 0.2);
          graph_->SetNodeAttr("TrackSyncNode", "human.rect.shrink.right", 0.2);
          graph_->SetNodeAttr("TrackSyncNode", "human.rect.shrink.top", 0.1);
          graph_->SetNodeAttr("TrackSyncNode", "human.rect.shrink.bottom", 0.6);
        }

        //设置graph内node连接
        {
          graph_->LinkNode("VideoInputNode", 0, "VideoInputNode2");
          graph_->LinkNode("VideoInputNode2", 0, "StreamReceiver");
          graph_->LinkNode("VideoInputNode2", 1, "VideoDecoderNode", 0); 
          
          if (local_stream_type_ == DE_PIX_FMT_MJPEG){
            graph_->LinkNode("VideoDecoderNode", 0, "JpegDecRsltHandle");
            graph_->LinkNode("JpegDecRsltHandle", 0, "TestVideoFrameFilter");
            graph_->LinkNode("JpegDecRsltHandle", 1, "TrackSyncNode");
          }
          else{
            graph_->LinkNode("VideoDecoderNode", 0, "TestVideoFrameFilter"); 
            graph_->LinkNode("VideoDecoderNode", 1, "TrackSyncNode");
          }
                
          graph_->LinkNode("TestVideoFrameFilter", 0, "MultiCatDetNode1");
          graph_->LinkNode("MultiCatDetNode1", 3, "TrackSyncNode");
          if (!encrypt) {
            graph_->LinkNode("TestVideoFrameFilter", 1, "MultiCatDetNode2");
            graph_->LinkNode("TestVideoFrameFilter", 2, "MultiCatDetNode3");
            graph_->LinkNode("TestVideoFrameFilter", 3, "MultiCatDetNode4");
            graph_->LinkNode("TestVideoFrameFilter", 4, "MultiCatDetNode5");    
            graph_->LinkNode("TestVideoFrameFilter", 5, "MultiCatDetNode6");
            graph_->LinkNode("MultiCatDetNode2", 3, "TrackSyncNode");
            graph_->LinkNode("MultiCatDetNode3", 3, "TrackSyncNode");
            graph_->LinkNode("MultiCatDetNode4", 3, "TrackSyncNode");
            graph_->LinkNode("MultiCatDetNode5", 3, "TrackSyncNode");
            graph_->LinkNode("MultiCatDetNode6", 3, "TrackSyncNode"); 
          }
          graph_->LinkNode("TrackSyncNode", 5, "NnReceiver", 1);
        }

        //设置graph输入和输出
        {
          graph_->SetOutputNode(0, "StreamReceiver", 0);
          graph_->SetOutputNode(1, "NnReceiver", 0);
        }
        graph_->ToFile("tmp.json");
      }
    }

    ~VideoPredict(){
      UnloadModel();
    }

    void Start(){
      if (remote_disp_){
        char buf[64];
        sprintf(buf, "ws://localhost:%d", ws_port_);
        while ( -1 == (sessionid_ = if_ws_client_connect(buf,"config", onopen, NULL, onmsg, onclose))) continue;
      }
      else
      {
        local_disp_ = new MediaDisplay();
      }
      //启动业务
      graph_->Start();
      //启动stream读取线程，获取graph执行结果
      std::thread([this](de::Graph *_graph) {
        void *p;
        int64_t frame_cnt = 0;
        while (nullptr != (p = _graph->GetOutput(0))) {
          de::MediaTask *task = static_cast<de::MediaTask*>(p);
          //LOG(INFO) << "<====stream result " << task->pts << " size " << task->array.GetTensorDataSize();
          
          {
            if_image_t image;
            int height,width, format;
            task->array.GetImgInfo(height, width, format);
            image.height = height;
            image.width = width;
            image.time_stamp = task->pts; 
            image.size = task->array.GetTensorDataSize();           
            image.extra_data[0] = nullptr;
            image.extra_data[1] = nullptr;
            image.format = local_stream_type_;
            if (remote_disp_ ) {
              image.data = (uint8_t*)task->array.GetTensorData();
              media_session_map[sessionid_]->src_frame_send((uint8_t*)image.data,image.size,image.width,image.height,image.time_stamp,local_stream_type_);
            } else {
              if (image.format == DE_PIX_FMT_MJPEG){
                std::vector<char> vecImg((char*)(task->array.GetTensorData()), (char*)(task->array.GetTensorData())+image.size);
                
                cv::Mat mat = cv::imdecode(vecImg, CV_LOAD_IMAGE_COLOR); 
                image.size = mat.elemSize() * mat.rows * mat.cols;
                image.data = new uint8_t[image.size];
                image.width = mat.cols;
                image.height = mat.rows;
                image.format = IF_IMG_BGR888;
                memcpy(image.data, mat.data, image.size);
              }else{
                image.data = new uint8_t[image.size];
                memcpy(image.data, task->array.GetTensorData(), image.size);
              }
              SPImage image_ptr(new Image(image));
              local_disp_->feed_frame(image_ptr);   
            }
          }

          frame_cnt++;
          delete task;
        }
      }, graph_).detach();

      //track结果读取线程，获取graph执行结果
      std::thread([this](de::Graph *_graph) {
        void *p;
        while (nullptr != (p = _graph->GetOutput(1))) {
          arctern::MultiTrackResult *task_in = static_cast<arctern::MultiTrackResult*>(p);
          // LOG(INFO) << "<===track result streamid " << task_in->stream_id << " frameid " << task_in->frm_id  << " frameid pts "<< task_in->pts  << " size " << task_in->obj_boxes.size();
         
          IfWsFrame frame;      
          frame.frame_id = task_in->frm_id;
          //frame.stream_id = task_in->stream_id;
          frame.time_stamp = task_in->pts;
          frame.type = 1;
      
          //端到端延时
          timespec time1;
          clock_gettime(CLOCK_MONOTONIC,&time1);
          uint64_t delay = (time1.tv_sec*1000000000 + time1.tv_nsec)/1000000 - frame.time_stamp; 
          frame.delay = delay;
		  printf("============= det delay %lld =============\n",delay);

          for (auto itr_boxs = task_in->obj_boxes.begin(); itr_boxs != task_in->obj_boxes.end(); itr_boxs++) {
            int32_t type = (int32_t)(itr_boxs->first);
            std::vector<de::PairIntRect> vec_rects = itr_boxs->second;
            for (auto boxs_num = 0; boxs_num < vec_rects.size(); boxs_num++) {
              IfWsObj obj;
              obj.rect.left   = vec_rects[boxs_num].second.x;
              obj.rect.top    = vec_rects[boxs_num].second.y;
              obj.rect.right  = vec_rects[boxs_num].second.x + vec_rects[boxs_num].second.w + 1;
              obj.rect.bottom = vec_rects[boxs_num].second.y + vec_rects[boxs_num].second.h + 1;     
              obj.type = type;
              frame.objs.emplace_back(obj);
            }
          }
          if (remote_disp_) 
            media_session_map[sessionid_]->frame_send(frame);
          else
          {
            DisplayInfo data;
            ObjectDisplayInfo info;
            data.time_stamp = frame.time_stamp; 
            data.frame_id = frame.frame_id;
            data.trk_delay = frame.delay;
            int obj_num = frame.objs.size();
            DEMO_LOG_INFO("frame info: frametype %d, frameid %d, timestamp %ld, obj_num %d\n", frame.type, frame.frame_id, frame.time_stamp, obj_num);
            for (int i = 0; i < obj_num; i++) {
              //printf("[%s %d %d %d %d %d]\n",frame.obj[i].attrs.c_str(), frame.obj[i].track_id, frame.obj[i].rect.left,frame.obj[i].rect.top,frame.obj[i].rect.right,frame.obj[i].rect.bottom);
              info.rect.left = frame.objs[i].rect.left;
              info.rect.right = frame.objs[i].rect.right;
              info.rect.top = frame.objs[i].rect.top;
              info.rect.bottom = frame.objs[i].rect.bottom;
              info.side_text = frame.objs[i].attrs.c_str();
              info.top_text = "tid:" + std::to_string(frame.objs[i].track_id);
              data.obj_infos.emplace_back(info);
            }
            local_disp_->feed_data(data);
          }       
          delete task_in;
        }
      }, graph_).detach();
    
    }
    
  private:
    int LoadModel(bool encrypt=false){
      std::string net_bin_, model_bin_;

      de::ModelManager mem(dev_id_);
      net_bin_ = model_path_ + "/net.bin";
      model_bin_ = model_path_ + "/model.bin";
      mem.Load(net_bin_, model_bin_, encrypt);

      net_bins_.emplace_back(net_bin_);
      model_bins_.emplace_back(model_bin_);
      LOG(INFO)<< "model " << net_bin_ << "download success.";

      return 0;
    }

    int UnloadModel(){
      de::ModelManager mem(dev_id_);

      for (auto idx = 0; idx < net_bins_.size(); idx++)
      {
        mem.Unload(net_bins_[idx]);
      }

      return 0;
    }

    int dev_id_;
    int stream_id_;
    std::string uri_;
    de::Graph* graph_;
    std::string model_path_;
    std::vector<std::string> net_bins_;
    std::vector<std::string> model_bins_;
    int ws_port_ = 9002;

    bool remote_disp_ = true;
    MediaDisplay* local_disp_ = nullptr;
    int sessionid_ = -1;
  int local_stream_type_ = DE_PIX_FMT_H264;
};

};


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

std::vector<std::string> vStringSplit(const std::string& s, const std::string& delim=",")
{
  std::vector<std::string> elems;
  size_t pos = 0;
  size_t len = s.length();
  size_t delim_len = delim.length();
  if (delim_len == 0) return elems;
  while (pos < len)
  {
    int find_pos = s.find(delim, pos);
    if (find_pos < 0)
    {
      elems.push_back(s.substr(pos, len - pos));
      break;
    }
    elems.push_back(s.substr(pos, find_pos - pos));
    pos = find_pos + delim_len;
  }
  return elems;
}

int main(int argc, char *argv[]) {

  signal(SIGTERM, signal_exit_handler);
  signal(SIGINT, signal_exit_handler);
  #ifdef DEBUG_ABORT_TRACE_ON
  signal(SIGSEGV, trace);
  signal(SIGABRT, trace);
  #endif

  std::vector<std::string> video_uri;
  std::vector<de::VideoPredict*> vdemos;
  std::vector<void*> vhandles;
  
  int display_mode = 0; //0: display in another process 1:display in current process
  int display_width = 1920;
  int display_height = 1080;
  int dev_cnt = atoi(argv[1]);
  int stream_cnt  = atoi(argv[2]);
  std::string devlib_path = argv[3];
  std::string model_path = argv[4];
  int encrypt = atoi(argv[5]); //encrypted model or not
  int stream_type = atoi(argv[6]); //33-MJPEG 31-h264
  int stream_width = atoi(argv[7]);
  int stream_height = atoi(argv[8]);
  int stream_fps = atoi(argv[9]);
  
  std::vector<std::string> dev_libs = vStringSplit(devlib_path,",");
  
  if (argc > 10) {
    for (auto idx = 0; idx < stream_cnt; idx++) {
      video_uri.emplace_back(argv[10 + idx]);
    }
  }
  
  if (argc > 10 + stream_cnt) display_mode = atoi(argv[10+stream_cnt]);
  
  if (argc > 10 + stream_cnt + 2) {
    display_width = atoi(argv[11+stream_cnt]);
    display_height = atoi(argv[12+stream_cnt]);
  }
  
  if (display_mode == 1) MediaDisplay::config("demo", display_width, display_height, dev_cnt*stream_cnt);

  for (int dev_id = 0; dev_id < dev_cnt; dev_id++) {
    for (int i=0;i<dev_libs.size();i++){ 
      void* handle = de::Device::LoadLib(dev_id, dev_libs[i]);
      vhandles.emplace_back(handle);
  }
    for (int stream_id = 0; stream_id < stream_cnt; stream_id++) {
      de::VideoPredict* demo = new de::VideoPredict("demo", dev_id, dev_id*stream_cnt + stream_id, video_uri[stream_id], model_path, 9002, stream_type,stream_width,stream_height,stream_fps,encrypt,display_mode);
      vdemos.emplace_back(demo);
    }
  }

  for (int i = 0; i < vdemos.size(); i++){
    vdemos[i]->Start();
  }

  while(!gExitFlag)
  {
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }

  for (int i = 0; i < vdemos.size(); i++){
    delete vdemos[i];
  }

  for (int i = 0; i < vhandles.size(); i++){
    de::Device::UnLoadLibHandle(i,vhandles[i]);
  }
  
  return 0;
}
