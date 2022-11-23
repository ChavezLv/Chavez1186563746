#include "yolo_det.h"
// #include "de_arctern_manager.h"
//#include "yolo_post_proc.h"
#include "de_arctern_node.h"
#include <core/de_node.h>


namespace de {
namespace arctern {

/**
* @brief: model pre-process function
* @param [in] img  image data
* @param [in] self YoloDet object pointer
* @retval model tensor inputs
*/
std::vector<de::NDArray> YoloDet::PreProc(const de::NDArray& img, const YoloDet* self) {
    std::vector<de::NDArray> outs;
	outs.push_back(img);
	return outs;
}

std::vector<de::arctern::MultiCatDetection> YoloDet::MultiPostProc(const std::vector<de::NDArray>& outs, const int32_t img_w, const int32_t img_h, const YoloDet* self)
{
    //CHECK(outs.size() == 3) << "YoloDet outs.size != 3";    
    std::vector<de::arctern::MultiCatDetection> result;
    result = self->yolov3_post_proc.run_multi_cat(outs, img_w, img_h);
    return result;
}

YoloDet::YoloDet(bool stream_mode) : AiEngine(stream_mode) {
	///< set default para
    attr_.SetDefault("yolo.type", 2); //E_YOLO_TYPE: 0: E_YOLO_IF face-det-video-0.0/0.3 1:E_YOLO_V3_V4 2:E_YOLO_V3_TINY 
    attr_.SetDefault("det.class.num", 1);
	attr_.Set("resize_type", 1);

    for (int i = 0; i < 4; i++)
    {
        std::string str = "roi" + std::to_string(i);
        attr_.SetDefault(str + ".x", 0, 0);
        attr_.SetDefault(str + ".y", 0, 0);
        attr_.SetDefault(str + ".w", 0, 0);
        attr_.SetDefault(str + ".h", 0, 0);
    }

    name_ = "YoloDet";
}

YoloDet::~YoloDet() {
}

}
	
}




