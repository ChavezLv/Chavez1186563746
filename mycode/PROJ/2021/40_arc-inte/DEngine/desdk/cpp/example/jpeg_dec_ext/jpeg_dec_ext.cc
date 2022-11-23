#include <iostream>
#include <thread>
#include <fstream>
#include "de_node.h"
#include "de_graph_api.h"
#include <de_thread.h>
#include "de_pic_decoder_node.h"
#include "de_node_common.h"


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
  jpeg_in   : "/DEngine/desdk/data/test/test.jpg",
  file_out  : "/DEngine/desdk/data/test/test.yuv",
  width     : 1920,
  height    : 1080,
  format    : DE_PIX_FMT_MJPEG,
}; 

//发送node，透传数据到设备node
class Sender : public de::Thread {
  public:
  Sender(){
    pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>,de::TaskDeleter<de::MediaTask>);
  }

  virtual ~Sender() {}

  virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT){
    printf("Sender Proc step000 task_type %d pout_type %d\n", task_type,pout_type);
    de::MediaTask* pTask = static_cast<de::MediaTask*>(rx_task);
      FILE *file = fopen("tmp.jpg", "w");
      fwrite((char*)pTask->array.GetTensorData(), pTask->array.GetTensorDataSize(), 1, file);
      fclose(file);

    if (pout_type == DEFAULT_POUT)
        pouts_[0]->SendTask(pTask);
    else
        vir_pouts_[0]->SendTask(pTask);
  }
  virtual void Stop(void){pin.DestroyQueue();}
};

/*
typedef struct {
    de::MediaTask task;
    std::string picname;
} MediaTaskExt;

namespace de{
namespace serializer{
    STRUCT_SERIALIZE_2(MediaTaskExt,
    de::MediaTask, task,
    std::string, picname);
}
}
*/

typedef struct{
   de::NDArray  array;     ///< data object
   int    streamid;  ///< stream id used for multi-stream
   uint64_t pts;     ///< timestamp
   uint32_t frameid;   ///< frame id
   char picname[32];   ///pic name

   inline de::NDArray Serialize() {
     int32_t *ptr = static_cast<int32_t*>(array.GetBearData());
     ptr[0] = streamid;
     ptr[1] = pts & 0xFFFFFFFF;
     ptr[2] = (pts >> 32) & 0xFFFFFFFF;
     ptr[3] = frameid;
       memcpy((char *)ptr + 16, picname, 32);
     std::vector<int64_t> shapes;
     array.Shrink(shapes, 12*sizeof(int32_t));
     return array;
   }

   inline void DeSerialize(de::NDArray data) {
     int32_t *ptr = static_cast<int32_t*>(data.GetBearData());
     streamid = ptr[0];
     pts = (((int64_t)ptr[2])<<32) | ((int64_t)ptr[1] & 0xFFFFFFFF);
     frameid = ptr[3];
       memcpy(picname, (char *)ptr + 16, 32);
     array = data;
   }
} MediaTaskExt;

typedef struct{
   de::NDArray  array;     ///< data object
   int    streamid;  ///< stream id used for multi-stream
   uint64_t pts;     ///< timestamp
   uint32_t frameid;   ///< frame id
   char picname[32];   ///pic name
   int  error;
   inline de::NDArray Serialize() {
     int32_t *ptr = static_cast<int32_t*>(array.GetBearData());
     ptr[0] = streamid;
     ptr[1] = pts & 0xFFFFFFFF;
     ptr[2] = (pts >> 32) & 0xFFFFFFFF;
     ptr[3] = frameid;
       memcpy((char *)ptr + 16, picname, 32);
       ptr[12] = error;
       
     std::vector<int64_t> shapes;
     array.Shrink(shapes, 13*sizeof(int32_t));
     return array;
   }

   inline void DeSerialize(de::NDArray data) {
     int32_t *ptr = static_cast<int32_t*>(data.GetBearData());
     streamid = ptr[0];
     pts = (((int64_t)ptr[2])<<32) | ((int64_t)ptr[1] & 0xFFFFFFFF);
     frameid = ptr[3];
       memcpy(picname, (char *)ptr + 16, 32);
       error = ptr[12];
     array = data;
   }
} MediaTaskExt2;

class SenderExt : public Sender{
public:
    void VirPout0TaskConvert(void* rx_task, int rx_task_type, void* out_task, de::POUTBase** pout)
    {
        switch (rx_task_type)
        {
        case 1:
        {
            //取出基类的输出＋派生类的输入
            de::MediaTask* p_base_out = static_cast<de::MediaTask *>(out_task);
            MediaTaskExt* p_ext_in = static_cast<MediaTaskExt*>(rx_task);

            //构造派生类的输出
            MediaTaskExt* p_ext_out = new MediaTaskExt;
            p_ext_out->array = p_base_out->array;
            p_ext_out->streamid = p_base_out->streamid;
            p_ext_out->pts = p_base_out->pts;
            p_ext_out->frameid = p_base_out->frameid;
            memcpy(p_ext_out->picname, p_ext_in->picname, 32);

            //发送派生类的输出
            pout[1]->SendTask(p_ext_out);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task type " << rx_task_type;
            break;
        }
        }
    }

    SenderExt()
    {
        pin.SetTypeInfo(1, "MediaTaskExt", de::TaskDeSerializeCustom<MediaTaskExt>, de::TaskDeleter<MediaTaskExt>);
        pouts_[1]->SetTypeInfo("MediaTaskExt", de::TaskSerializeCustom<MediaTaskExt>,de::TaskDeleter<MediaTaskExt>);

        VPOUT_CONVERCT_FUNC(0, SenderExt, VirPout0TaskConvert);
    }

    ~SenderExt() 
    {
    
    }

    
    virtual void Proc(void* task, int task_type, POutType pout_type = DEFAULT_POUT) override
    {
    printf("SenderExt Proc step000 task_type %d pout_type %d\n",task_type, pout_type);
        de::POUTBase** pout = SelectPout(pout_type);

        switch(task_type)
        {
        case 0:
        {
            //基类可直接处理的，交由基类处理，参数3传递时直接使用pout
            Sender::Proc(task, task_type);
            break;
        }
        case 1: //type 1是对type 0的扩展。转成type 0类型的任务，交由基类处理
        {
            void* p_base_task = PinConvert(task, task_type);
            Sender::Proc(p_base_task, 0, USER_POUT);
            VirPoutTaskProc(task, task_type, pout);
            pin.DelTask(task, task_type); 
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task_type " << task_type;
            break;
        }
        }
    }


    virtual void Stop(void) override
    {
        pin.DestroyQueue();
    }


private:
    
    void* PinConvert(void* rx_task, int rx_task_type)
    {
        switch (rx_task_type)
        {
        case 1:
        {
            printf("SenderExt Proc PinConvert\n");
            de::MediaTask* p_base_task = new de::MediaTask;
            MediaTaskExt* p_ext_task = static_cast<MediaTaskExt *>(rx_task);
            p_base_task->array = p_ext_task->array;
            p_base_task->streamid = p_ext_task->streamid;
            p_base_task->pts = p_ext_task->pts;
            p_base_task->frameid = p_ext_task->frameid;

            return p_base_task;
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown rx_task_type " << rx_task_type;
            break;
        }
        }

        return nullptr;
    }
};

class SenderExt2 : public SenderExt{
public:
    void VirPout0TaskConvert(void* rx_task, int rx_task_type, void* out_task, de::POUTBase** pout)
    {
        switch (rx_task_type)
        {
        case 2:
        {
            //取出基类的输出＋派生类的输入
            MediaTaskExt* p_base_out = static_cast<MediaTaskExt *>(out_task);
            MediaTaskExt2* p_ext_in = static_cast<MediaTaskExt2*>(rx_task);
            //构造派生类的输出
            MediaTaskExt2* p_ext_out = new MediaTaskExt2;
            p_ext_out->array = p_base_out->array;
            p_ext_out->streamid = p_base_out->streamid;
            p_ext_out->pts = p_base_out->pts;
            p_ext_out->frameid = p_base_out->frameid;
            memcpy(p_ext_out->picname, p_base_out->picname, 32);
            p_ext_out->error = p_ext_in->error;
            //发送派生类的输出
            pout[2]->SendTask(p_ext_out);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task type " << rx_task_type;
            break;
        }
        }
    }

    SenderExt2()
    {
        pin.SetTypeInfo(2, "MediaTaskExt2", de::TaskDeSerializeCustom<MediaTaskExt2>, de::TaskDeleter<MediaTaskExt2>);
        pouts_[2]->SetTypeInfo("MediaTaskExt2", de::TaskSerializeCustom<MediaTaskExt2>);

        VPOUT_CONVERCT_FUNC(0, SenderExt2, VirPout0TaskConvert);
    }

    ~SenderExt2() 
    {
    
    }

    
    virtual void Proc(void* task, int task_type, POutType pout_type = DEFAULT_POUT) override
    {
    printf("SenderExt2 Proc step000\n");
        de::POUTBase** pout = SelectPout(pout_type);
printf("SenderExt2 Proc  step111 pout_type %d task_type %d\n",pout_type,task_type);
        switch(task_type)
        {
        case 0:
        {
            //基类可直接处理的，交由基类处理，参数3传递时直接使用pout
            SenderExt::Proc(task, task_type);
            break;
        }
        case 1: //type 1是对type 0的扩展。转成type 0类型的任务，交由基类处理
        {
            void* p_base_task = PinConvert(task, task_type);

            SenderExt::Proc(p_base_task, 0, USER_POUT);

            VirPoutTaskProc(task, task_type, pout);

            pin.DelTask(task, task_type);            
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task_type " << task_type;
            break;
        }
        }
    }


    virtual void Stop(void) override
    {
        pin.DestroyQueue();
    }


private:
    
    void* PinConvert(void* rx_task, int rx_task_type)
    {
        switch (rx_task_type)
        {
        case 2:
        {
            MediaTaskExt* p_base_task = new MediaTaskExt;
            MediaTaskExt2* p_ext_task = static_cast<MediaTaskExt2 *>(rx_task);
            p_base_task->array = p_ext_task->array;
            p_base_task->streamid = p_ext_task->streamid;
            p_base_task->pts = p_ext_task->pts;
            p_base_task->frameid = p_ext_task->frameid;
            memcpy(p_base_task->picname, p_ext_task->picname, 32);

            return p_base_task;
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown rx_task_type " << rx_task_type;
            break;
        }
        }

        return nullptr;
    }
};


//接收node，从设备node透传数据
class Receiver : public de::Thread {
public:
  Receiver(){
    pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
  }

   virtual ~Receiver(){}
  
   virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT)
   {
        de::MediaTask* pTask = static_cast<de::MediaTask*>(rx_task);

        if (pout_type == DEFAULT_POUT)
            pouts_[0]->SendTask(pTask);
        else
            vir_pouts_[0]->SendTask(pTask);
    }

  virtual void Stop(void){pin.DestroyQueue();}

};


class ReceiverExt : public Receiver{
public:
    void VirPout0TaskConvert(void* rx_task, int rx_task_type, void* out_task, de::POUTBase** pout)
    {
        switch (rx_task_type)
        {
        case 1:
        {
            //取出基类的输出＋派生类的输入
            de::MediaTask* p_base_out = static_cast<de::MediaTask *>(out_task);
            MediaTaskExt* p_ext_in = static_cast<MediaTaskExt*>(rx_task);

            //构造派生类的输出
            MediaTaskExt* p_ext_out = new MediaTaskExt;
            p_ext_out->array = p_base_out->array;
            p_ext_out->streamid = p_base_out->streamid;
            p_ext_out->pts = p_base_out->pts;
            p_ext_out->frameid = p_base_out->frameid;
            memcpy(p_ext_out->picname, p_ext_in->picname, 32);

            //发送派生类的输出
            pout[1]->SendTask(p_ext_out);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task type " << rx_task_type;
            break;
        }
        }
    }

    ReceiverExt()
    {
        pin.SetTypeInfo(1, "MediaTaskExt", de::TaskDeSerializeCustom<MediaTaskExt>, de::TaskDeleter<MediaTaskExt>);
        pouts_[1]->SetTypeInfo("MediaTaskExt", de::TaskSerializeCustom<MediaTaskExt>, de::TaskDeleter<MediaTaskExt>);

        VPOUT_CONVERCT_FUNC(0, ReceiverExt, VirPout0TaskConvert);
    }

    virtual ~ReceiverExt()
    {
    }

    virtual void Stop(void){pin.DestroyQueue();}

    
    virtual void Proc(void* task, int task_type, POutType pout_type = DEFAULT_POUT) override
    {
        de::POUTBase** pout = SelectPout(pout_type);

        switch(task_type)
        {
        case 0:
        {
            //基类可直接处理的，交由基类处理，参数3传递时直接使用pout
            Receiver::Proc(task, task_type);
            break;
        }
        case 1: //type 1是对type 0的扩展。转成type 0类型的任务，交由基类处理
        {
            void* p_base_task = PinConvert(task, task_type);
            Receiver::Proc(p_base_task, 0, USER_POUT);

            VirPoutTaskProc(task, task_type, pout);
            pin.DelTask(task, task_type);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task_type " << task_type;
            break;
        }
        }
    }

private:
    
    void* PinConvert(void* rx_task, int rx_task_type)
    {
        switch (rx_task_type)
        {
        case 1:
        {
            de::MediaTask* p_base_task = new de::MediaTask;
            MediaTaskExt* p_ext_task = static_cast<MediaTaskExt *>(rx_task);
            p_base_task->array = p_ext_task->array;
            p_base_task->streamid = p_ext_task->streamid;
            p_base_task->pts = p_ext_task->pts;
            p_base_task->frameid = p_ext_task->frameid;

            return p_base_task;
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown rx_task_type " << rx_task_type;
            break;
        }
        }

        return nullptr;
    }
};

class ReceiverExt2 : public ReceiverExt{
public:
    void VirPout0TaskConvert(void* rx_task, int rx_task_type, void* out_task, de::POUTBase** pout)
    {
        switch (rx_task_type)
        {
        case 2:
        {
            //取出基类的输出＋派生类的输入
            MediaTaskExt* p_base_out = static_cast<MediaTaskExt *>(out_task);
            MediaTaskExt2* p_ext_in = static_cast<MediaTaskExt2*>(rx_task);

            //构造派生类的输出
            MediaTaskExt2* p_ext_out = new MediaTaskExt2;
            p_ext_out->array = p_base_out->array;
            p_ext_out->streamid = p_base_out->streamid;
            p_ext_out->pts = p_base_out->pts;
            p_ext_out->frameid = p_base_out->frameid;
            memcpy(p_ext_out->picname, p_ext_in->picname, 32);
            p_ext_out->error = p_ext_in->error;
            //发送派生类的输出
            pout[2]->SendTask(p_ext_out);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task type " << rx_task_type;
            break;
        }
        }
    }

    ReceiverExt2()
    {
        pin.SetTypeInfo(2, "MediaTaskExt2", de::TaskDeSerializeCustom<MediaTaskExt2>, de::TaskDeleter<MediaTaskExt2>);
        pouts_[2]->SetTypeInfo("MediaTaskExt2", de::TaskSerializeCustom<MediaTaskExt2>);

        VPOUT_CONVERCT_FUNC(0, ReceiverExt, VirPout0TaskConvert);
    }

    virtual ~ReceiverExt2()
    {
    }

    virtual void Stop(void){pin.DestroyQueue();}

    
    virtual void Proc(void* task, int task_type, POutType pout_type = DEFAULT_POUT) override
    {
        de::POUTBase** pout = SelectPout(pout_type);

        switch(task_type)
        {
        case 0:
        {
            //基类可直接处理的，交由基类处理，参数3传递时直接使用pout
            ReceiverExt::Proc(task, task_type);
            break;
        }
        case 1: //type 1是对type 0的扩展。转成type 0类型的任务，交由基类处理
        {
            void* p_base_task = PinConvert(task, task_type);
            ReceiverExt::Proc(p_base_task, 0, USER_POUT);

            VirPoutTaskProc(task, task_type, pout);
            pin.DelTask(task, task_type);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task_type " << task_type;
            break;
        }
        }
    }

private:
    
    void* PinConvert(void* rx_task, int rx_task_type)
    {
        switch (rx_task_type)
        {
        case 1:
        {
            MediaTaskExt* p_base_task = new MediaTaskExt;
            MediaTaskExt2* p_ext_task = static_cast<MediaTaskExt2 *>(rx_task);
            p_base_task->array = p_ext_task->array;
            p_base_task->streamid = p_ext_task->streamid;
            p_base_task->pts = p_ext_task->pts;
            p_base_task->frameid = p_ext_task->frameid;
            memcpy(p_base_task->picname, p_ext_task->picname, 32);

            return p_base_task;
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown rx_task_type " << rx_task_type;
            break;
        }
        }

        return nullptr;
    }
};


namespace de
{
DE_CLASS_REGISTER("SenderExt", SenderExt);
DE_CLASS_REGISTER("ReceiverExt", ReceiverExt);
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

  void* handle = de::Device::LoadLib(0,"host:/DEngine/desdk/platform/dev_linux-dp1000/lib/libjpeg_dec_dev_ext.so");

  //创建graph
  de::Graph graph("jpeg_dec_ext");

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
  printf("step0\n");
  graph.CreateHostNode("SenderExt", "jpeg_in_ext");
  graph.CreateHostNode("ReceiverExt", "yuv_out_ext");
  graph.CreateDevNode("JpegDecoderExt", "jpeg_dec_ext");

  //设置graph内node连接，jpeg_in的Pout[0]连接到jpeg_dec的Pin，使用bridge通道0
  graph.LinkNode("jpeg_in_ext", 1, "jpeg_dec_ext", 0);
  //设置graph内node连接，jpeg_dec的Pout[0]连接yuv_out的Pin，使用bridage通道1
  graph.LinkNode("jpeg_dec_ext", 4, "yuv_out_ext", 1);

  //设置graph输出，graph的Pout[0]为yuv_out的Pout[0]
  graph.SetOutputNode(0, "yuv_out_ext", 1);
  //设置graph输入，graph的Pin[0]为jpeg_in的Pin
  graph.SetInputNode(0, "jpeg_in_ext");

  //启动业务
  graph.Start();

  //启动结果读取线程，获取graph执行结果
  std::thread([](de::Graph *graph) {
    void *p;
    while (nullptr != (p = graph->GetOutput(0))) {
      MediaTaskExt *task = static_cast<MediaTaskExt*>(p);
      LOG(INFO) << "jpeg ====> yuv " << " decode finished.";
      FILE *file = fopen(kCaseCfg.file_out.c_str(), "w");
      fwrite((char*)task->array.GetTensorData(), task->array.GetTensorDataSize(), 1, file);
      delete task;
    }
  }, &graph).detach();

  //输入jpeg数据
  de::NDArray image = get_image(kCaseCfg.format, kCaseCfg.width, kCaseCfg.height,  kCaseCfg.jpeg_in.c_str(), nullptr);
  for (int i = 0; i < 1; i++) {
      MediaTaskExt* p = new MediaTaskExt;
      p->array = image;
      p->streamid = 1234;
      p->pts = 5555;
      strcpy(p->picname, "test.jpg");
      graph.SetInput(0, p, 1);
  }

  std::this_thread::sleep_for(std::chrono::seconds(15));
  //确保graph的output队列退出
  graph.StopOutput<de::MediaTask>(0);

  de::Device::UnLoadLibHandle(0,handle);

  return 0;
}
