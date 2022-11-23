#include <iostream>
#include <thread>
#include <fstream>
#include "de_node.h"
#include "de_graph_api.h"
#include <de_thread.h>
#include "de_pic_decoder_node.h"
#include "de_node_common.h"

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


class DE_DLL JpegDecExt : public de::mm::JpegDecoder 
{

public:
    void VirPout0TaskConvert(void* rx_task, int rx_task_type, void* out_task, de::POUTBase** pout)
    {
        printf("JpegDecExt VirPout0TaskConvert rx_task_type %d \n",rx_task_type);
        switch (rx_task_type)
        {
        case 1:
        {
            printf("JpegDecExt VirPout0TaskConvert\n");
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
            pout[4]->SendTask(p_ext_out);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task type " << rx_task_type;
            break;
        }
        }
    }

    JpegDecExt()
    {
        pin.SetTypeInfo(1, "MediaTaskExt", de::TaskDeSerializeCustom<MediaTaskExt>, de::TaskDeleter<MediaTaskExt>);
        pouts_[4]->SetTypeInfo("MediaTaskExt", de::TaskSerializeCustom<MediaTaskExt>,de::TaskDeleter<MediaTaskExt>);

        VPOUT_CONVERCT_FUNC(0, JpegDecExt, VirPout0TaskConvert);
    }

    virtual ~JpegDecExt() {}

    virtual void Stop(void)
    {
        pin.DestroyQueue();
    }
    
    virtual void Proc(void* task, int task_type, POutType pout_type = DEFAULT_POUT) override
    {
        printf("JpegDecExt Proc step000 task_type %d pout_type %d\n",task_type, pout_type);
        de::POUTBase** pout = SelectPout(pout_type);

        switch(task_type)
        {
        case 0:
        {
            //基类可直接处理的，交由基类处理，参数3传递时直接使用pout
            de::mm::JpegDecoder::Proc(task, task_type, DEFAULT_POUT);
            break;
        }
        case 1: //type 1是对type 0的扩展。转成type 0类型的任务，交由基类处理
        {
            printf("JpegDecExt Proc case 1\n");
            void* p_base_task = PinConvert(task, task_type);
            de::mm::JpegDecoder::Proc(p_base_task, 0, USER_POUT);

            VirPoutTaskProc(task, task_type, pout);
            pin.DelTask(task, task_type);
            printf("JpegDecExt Proc case 1 end\n");
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


class DE_DLL JpegDecExt2 : public JpegDecExt
{

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
            strcpy(p_ext_out->picname, "test.jpg");
            p_ext_out->error = 1;
            //发送派生类的输出
            pout[5]->SendTask(p_ext_out);
            break;
        }
        default:
        {
            LOG(FATAL)<< "unknown task type " << rx_task_type;
            break;
        }
        }
    }

    JpegDecExt2()
    {
        pin.SetTypeInfo(2, "MediaTaskExt2", de::TaskDeSerializeCustom<MediaTaskExt2>, de::TaskDeleter<MediaTaskExt2>);
        pouts_[5]->SetTypeInfo("MediaTaskExt2", de::TaskSerializeCustom<MediaTaskExt2>);

        VPOUT_CONVERCT_FUNC(0, JpegDecExt2, VirPout0TaskConvert);
    }

    virtual ~JpegDecExt2() {}

    virtual void Stop(void)
    {
        pin.DestroyQueue();
    }
    
    virtual void Proc(void* task, int task_type, POutType pout_type = DEFAULT_POUT) override
    {
        de::POUTBase** pout = SelectPout(pout_type);

        switch(task_type)
        {
        case 0:
        {
            //基类可直接处理的，交由基类处理，参数3传递时直接使用pout
            JpegDecExt::Proc(task, task_type, DEFAULT_POUT);
            break;
        }
        case 1: //type 1是对type 0的扩展。转成type 0类型的任务，交由基类处理
        {
            void* p_base_task = PinConvert(task, task_type);
            JpegDecExt::Proc(p_base_task, 0, USER_POUT);

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


DE_CLASS_REGISTER("JpegDecoderExt", JpegDecExt);


