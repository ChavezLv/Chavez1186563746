#ifndef YOLO_DET_H
#define YOLO_DET_H
#include <core/de_ndarray.h>
#include <core/de_thread.h>
#include <node/de_pic_decoder_node.h>
#include <node/de_video_decoder_node.h>
#include <node/de_video_in_node.h>
#include <node/de_aiengine_node.h>
#include <de_arctern_node.h>
#include "yolov3_post_proc.h"

namespace de
{
namespace arctern
{

/**
* @brief Yolo Detection (Intellif)
* Support thread stream-call and sync-call.
* 1. Stream-Call Mode (by graph API):
* 	a) PIN task-type:
* 	- de::mm::VideDecTask
* 	- de::JpegDecTask
* 	- de::mm::VideoInTask
* 	b) POUT task-type:
* 	- Pout[0]: de::arctern::DetRsltImg
* 	- Pout[1]: de::arctern::DetRslt
* 	c) paras (key - value), setting by graph's node attribute:
*		key			 Value		Description
*	-	yolo.type	   2        E_YOLO_TYPE: 0: E_YOLO_IF face-det-video-0.0/0.3 1:E_YOLO_V3_V4 2:E_YOLO_V3_TINY
*   -   det.class.num  1        num of detected classes by yolo detect
*	-   stream.id      1		data's stream id (from video)
*
* 2. Sync-Call Mode:
* @code
* 	de::NDArray img = de::arctern::ReadImg("/tmp/1.pic");
*	auto yolo_det = new de::arctern::YoloDet(false);
*	auto yolo_det->Set("yolo.type", 2); // paras settings
*	auto yolo_det->Set("det.class.num", 1); // paras settings
*	auto yolo_det->Set("stream.id", 1); // paras settings
*	yolo_det->UpdatePara();
*	std::vector<std::pair<Rect, float>> result = yolo_det->Predict(img);
*	delete yolo_det;*
* @endcode
*
*/

class DE_DLL YoloDet : public de::AiEngine
{
public:
    YoloDet(bool stream_mode = true) ;
    ~YoloDet() ;

    ///< paras
    int yolo_type_ = 2; //E_YOLO_TYPE: 0: E_YOLO_IF face-det-video-0.0/0.3 1:E_YOLO_V3_V4 2:E_YOLO_V3_TINY
    int det_class_num_ = 1; //num of detected classes by yolo detect
    double conf_thresh_ = -1.0;
    //int32_t stream_id_ = 0;

	YoloV3PostProc yolov3_post_proc;

    static std::vector<de::NDArray> PreProc(const de::NDArray& img, const YoloDet* self);
    static std::vector<de::arctern::MultiCatDetection> MultiPostProc(const std::vector<de::NDArray>& outs, const int32_t img_w, const int32_t img_h, const YoloDet* self);

};

}; // namespace arctern

}; // namespace de


#endif
