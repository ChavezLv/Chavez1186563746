//#include <de_graph.h>
#include <core/de_node.h>
#include <node/de_pic_encoder_node.h>
#include "obj_yolov3_det.h"

namespace de
{
namespace arctern
{

/**
* @brief: stream input task pre-process function
* @param [in] rx_task input task
* @param [in] task_type input task type
* @param [in] self YoloDet object pointer
* @retval model tensor inputs
*/
static std::vector<de::NDArray> TaskPreProc(void* rx_task, int32_t task_type, void* self)
{
	static int counts = 0;
    de::NDArray img;
	std::vector<de::NDArray> outs;
    auto obj = static_cast<YoloDet*>(self);

    if (task_type == 0)
    {
        auto task = static_cast<de::MediaTask*>(rx_task);
    	img = task->array;
    	return YoloDet::PreProc(img, obj);
    }

	return outs;
}
template<typename T>
static std::pair<T*, de::NDArray> TaskPoutPostProc(void* rx_task, int32_t task_type, const std::vector<de::NDArray>& outs, void* self)
{
    auto tx_task = new T();
    auto obj = static_cast<YoloDet*>(self);    
    de::NDArray img;
 
	if(task_type == 0)
	{		
		auto task = static_cast<de::MediaTask*>(rx_task);
		tx_task->id = task->frameid;
		tx_task->stream_id = task->streamid;
		img = task->array;
		
        // obj->SetProfileInfo(task->frameid, img) ;
	}

    {
        int32_t img_w, img_h;
        GetImgNdWidthHeight(img, img_w, img_h);
        auto result = YoloDet::MultiPostProc(outs, img_w, img_h, obj);
        tx_task->rects = result;
    }
    return std::make_pair(tx_task, img);
}

/**
* @brief: stream output task post-process function (FaceDetRsltImg)
* @param [in] rx_task input task
* @param [in] task_type input task type
* @param [in] outs model tensor outputs
* @param [in] self YoloDet object pointer
* @retval stream output task
*/
static void* TaskPout0PostProc(void* rx_task, int32_t task_type, const std::vector<de::NDArray>& outs, void* self)
{
    auto ret = TaskPoutPostProc<MultiDetRsltImg>(rx_task, task_type, outs, self);
    auto tx_task = ret.first;
    auto img = ret.second;
    tx_task->img = img;
    return tx_task;
}


/**
* @brief: stream output task post-process function (FaceDetRslt)
* @param [in] rx_task input task
* @param [in] task_type input task type
* @param [in] outs model tensor outputs
* @param [in] self YoloDet object pointer
* @retval stream output task
*/
static void* TaskPout1PostProc(void* rx_task, int32_t task_type, const std::vector<de::NDArray>& outs, void* self)
{
    auto ret = TaskPoutPostProc<MultiDetRslt>(rx_task, task_type, outs, self);
    auto tx_task = ret.first;
    return tx_task;
}


ObjDet::ObjDet(bool stream_mode) : YoloDet(stream_mode)
{
    if(stream_mode_)
    {
        pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerialize<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
		//pin.SetTypeInfo(1, "de::JpegDecTask", de::TaskDeSerialize<de::JpegDecTask>, de::TaskDeleter<de::JpegDecTask>);
        pouts_[0]->SetTypeInfo("de::arctern::MultiDetRsltImg", de::TaskSerialize<de::arctern::MultiDetRsltImg>);
        pouts_[1]->SetTypeInfo("de::arctern::MultiDetRsltImg", de::TaskSerialize<de::arctern::MultiDetRsltImg>);
        pouts_[2]->SetTypeInfo("de::arctern::MultiDetRsltImg", de::TaskSerialize<de::arctern::MultiDetRsltImg>);
        pouts_[3]->SetTypeInfo("de::arctern::MultiDetRslt", de::TaskSerialize<de::arctern::MultiDetRslt>);
        pouts_[4]->SetTypeInfo("de::arctern::MultiDetRslt", de::TaskSerialize<de::arctern::MultiDetRslt>);
        pouts_[5]->SetTypeInfo("de::arctern::MultiDetRslt", de::TaskSerialize<de::arctern::MultiDetRslt>);

        SetPreProc(TaskPreProc);
        SetPostProc(0, TaskPout0PostProc);
		SetPostProc(1, TaskPout0PostProc);
		SetPostProc(2, TaskPout0PostProc);
        SetPostProc(3, TaskPout1PostProc);
		SetPostProc(4, TaskPout1PostProc);
		SetPostProc(5, TaskPout1PostProc);
    }
	attr_.Set("yolo.type", 0);

    name_ = "ObjDet";

}

ObjDet::~ObjDet()
{
}

void ObjDet::Stop(void){
    pin.DestroyQueue();
}

void ObjDet::UpdatePara(void) {
	
	YoloDet::UpdatePara();
}

PicObjDet::PicObjDet(bool stream_mode) : YoloDet(stream_mode)
{
    if(stream_mode_)
    {
        pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerialize<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
        pouts_[0]->SetTypeInfo("de::arctern::MultiDetRsltImg", de::TaskSerialize<de::arctern::MultiDetRsltImg>);
        pouts_[1]->SetTypeInfo("de::arctern::MultiDetRsltImg", de::TaskSerialize<de::arctern::MultiDetRsltImg>);
        pouts_[2]->SetTypeInfo("de::arctern::MultiDetRsltImg", de::TaskSerialize<de::arctern::MultiDetRsltImg>);
        pouts_[3]->SetTypeInfo("de::arctern::MultiDetRslt", de::TaskSerialize<de::arctern::MultiDetRslt>);
        pouts_[4]->SetTypeInfo("de::arctern::MultiDetRslt", de::TaskSerialize<de::arctern::MultiDetRslt>);
        pouts_[5]->SetTypeInfo("de::arctern::MultiDetRslt", de::TaskSerialize<de::arctern::MultiDetRslt>);

        SetPreProc(TaskPreProc);
        SetPostProc(0, TaskPout0PostProc);
		SetPostProc(1, TaskPout0PostProc);
		SetPostProc(2, TaskPout0PostProc);
        SetPostProc(3, TaskPout1PostProc);
		SetPostProc(4, TaskPout1PostProc);
		SetPostProc(5, TaskPout1PostProc);
    }
	attr_.Set("yolo.type", 0);

    name_ = "PicObjDet";

}

PicObjDet::~PicObjDet()
{
}

void PicObjDet::Stop(void){
    pin.DestroyQueue();
}

void PicObjDet::UpdatePara(void) {
	
	YoloDet::UpdatePara();
}



}

DE_CLASS_REGISTER("de::arctern::ObjDet", de::arctern::ObjDet);
DE_CLASS_REGISTER("de::arctern::PicObjDet", de::arctern::PicObjDet);
DE_CLASS_REGISTER("de::arctern::TestVideoFrameFilter", de::arctern::TestVideoFrameFilter);
DE_CLASS_REGISTER("de::arctern::TestPicDecFrameFilter", de::arctern::TestPicDecFrameFilter);
DE_CLASS_REGISTER("de::arctern::ObjDetRsltDispatcher", de::arctern::ObjDetRsltDispatcher);
DE_CLASS_REGISTER("de::arctern::JpegDecRsltHandle", de::arctern::JpegDecRsltHandle);
}

