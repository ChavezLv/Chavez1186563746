#include "yolo_det.h"

#include "node/de_node_common.h"

namespace de
{
namespace arctern
{
    
class DE_DLL ObjDet : public YoloDet
{
public:
    ObjDet(bool stream_mode = true);  
    ~ObjDet();

    void UpdatePara(void);
    void Stop(void) override;
    float *pOutSpace=nullptr;
};

class DE_DLL PicObjDet : public YoloDet
{
public:
    PicObjDet(bool stream_mode = true);  
    ~PicObjDet();

    void UpdatePara(void);
    void Stop(void) override;
    float *pOutSpace=nullptr;
};


class DE_DLL TestVideoFrameFilter : public de::Thread
{
public:
    TestVideoFrameFilter() {
         pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);

         pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
         pouts_[1]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
         pouts_[2]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
         pouts_[3]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
         pouts_[4]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
         pouts_[5]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
		 attr_.SetDefault("filter.period", 5, 1, 10000);
		 attr_.SetDefault("parallel_num", 1, 1, 100);
     }

    ~TestVideoFrameFilter() {}

     void UpdatePara(void){
        attr_.Get("filter.period", filter_period); 
		attr_.Get("parallel_num", parallel_num); 
        Thread::UpdatePara();
     }

    void Stop(void)
    {
        
    }

    virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) {
        
        if (task_type == 0)
        {
			de::MediaTask* p = (de::MediaTask*)rx_task;
			// printf("TestVideoFrameFilter pts %llu ndpts %llu frmid %d\n",p->pts,p->array.GetExtInfo()->pts,p->frameid);
            if (p->frameid%filter_period == 0)
            {
				if (parallel_num > 1){
					pouts_[index++]->SendTask(rx_task);
					index %= parallel_num;
				}else
					pouts_[0]->SendTask(rx_task);
            }
            else
            {
                pin.DelTask(rx_task, task_type);
            }
            counts++;
       }
    }

private:
    int filter_period;
	int parallel_num;
	int counts = 0;
    int index = 0;
};

class DE_DLL TestPicDecFrameFilter : public de::Thread
{
public:
    TestPicDecFrameFilter()
    {
        pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerialize<de::MediaTask>, de::TaskDeleter<de::MediaTask>);

        pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
        pouts_[1]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
        pouts_[2]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
        pouts_[3]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
        pouts_[4]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
        pouts_[5]->SetTypeInfo("de::MediaTask", de::TaskSerialize<de::MediaTask>);
		attr_.SetDefault("parallel_num", 1, 1, 100);
    }

    ~TestPicDecFrameFilter(){}

    void Stop(void){}

    void UpdatePara(void){
        attr_.Get("parallel_num", parallel_num); 
        Thread::UpdatePara();
    }

    virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) 
    {
        

        if (task_type == 0)
        {
			if (parallel_num > 1){
				pouts_[index++]->SendTask(rx_task);
				index %= parallel_num;
			}else
				pouts_[0]->SendTask(rx_task);
        }
    }

private:
	int parallel_num=1;
	int index = 0;
};

class DE_DLL ObjDetRsltDispatcher : public de::Thread
{
public:
    ObjDetRsltDispatcher() {
        pin.SetTypeInfo(0, "de::arctern::MultiDetRsltImg", de::TaskDeSerialize<de::arctern::MultiDetRsltImg>, de::TaskDeleter<de::arctern::MultiDetRsltImg>);
        pouts_[0]->SetTypeInfo("de::arctern::MultiDetRslt", de::TaskSerialize<de::arctern::MultiDetRslt>);
        pouts_[1]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
    }

    ~ObjDetRsltDispatcher() {}

    void Stop(void){}

    virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) {
        
        if (task_type == 0)
        {
            de::arctern::MultiDetRsltImg* pTask = static_cast<de::arctern::MultiDetRsltImg*>(rx_task);
            
            if (pTask->rects.size() > 0) {
                auto jpeg_task = new de::MediaTask();
                jpeg_task->array = pTask->img;
                jpeg_task->streamid = pTask->stream_id;
                jpeg_task->pts = pTask->id;
                jpeg_task->frameid = pTask->id;
                pouts_[1]->SendTask(jpeg_task);
            }

            auto multiDetRslt = new de::arctern::MultiDetRslt();
            multiDetRslt->id = pTask->id;
            multiDetRslt->rects = pTask->rects;
            multiDetRslt ->stream_id = pTask->stream_id;
            pouts_[0]->SendTask(multiDetRslt);
        }
        pin.DelTask(rx_task, task_type);
    }
};

class DE_DLL JpegDecRsltHandle : public de::Thread
{
public:
    JpegDecRsltHandle() {
        pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerialize<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
        pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
		pouts_[1]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
		attr_.SetDefault("width", 1920, 100, 4096);
		attr_.SetDefault("height", 1080, 100, 4096);
    }

    ~JpegDecRsltHandle() {}

    void Stop(void){}
	
	void UpdatePara(void){
        attr_.Get("width", width); 
		attr_.Get("height", height); 
        Thread::UpdatePara();
    }

    virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) {
        
        if (task_type == 0)
        {
            de::MediaTask* pTask = static_cast<de::MediaTask*>(rx_task);
            
            if (pTask->array.GetTensorDataSize() == 0) {   
                pTask->array = de::NDArray::Create( width,  height, dePixelFormat::DE_PIX_FMT_NV12, {0,0,width,height});
            }
			CHECK(pTask->array.defined());
			de::MediaTask* tx_task = new de::MediaTask;
				*tx_task = *pTask;
			if(pouts_[0]->IsLinked())
				pouts_[0]->SendTask(pTask);
			if(pouts_[1]->IsLinked()){
				pouts_[1]->SendTask(tx_task);
			}
        }
    }
	private:
	int width;
	int height;
};

}
}

