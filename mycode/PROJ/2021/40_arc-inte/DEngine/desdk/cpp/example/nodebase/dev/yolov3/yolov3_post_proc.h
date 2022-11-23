#ifndef YOLOV3_POST_PROC_H
#define YOLOV3_POST_PROC_H
#include <map>
#include <math.h>
#include <vector>
#include <limits>
#include <iostream>
#include <string>
#include "core/de_logging.h"
#include "de_arctern_utils.h"

namespace de
{
namespace arctern
{

typedef struct {
    float x, y, w, h;   
} box;

typedef struct detection {
    box bbox;
    int classes;
    float* prob;
    float* mask;
    float objectness;
    float top_occluded_prob;
    float bottom_occluded_prob;
    int sort_class;
} detection;

/*  Bounding box structure used for express a face area
*  xx1 : x coordinate of top_left
*  yy1: y coordinate of top_left
*  xx2 : x coordinate  of bot_right
*  yy2: y coordinate of bot_right
*  score:  likelihood of face
*  dxx1 : regression value of x coordinate of top_left
*  dyy1 : regression value of y coordinate of top_left
*  dxx2 : regression value of x coordinate of bot_right
*  dyy2 : regression value of y coordinate of bot_right
*  lmd: landmark
*/
struct Bbox {
    Bbox(int xx1, int yy1, int xx2, int yy2, float box_score, float dxx1 = 0.,
    float dyy1 = 0., float dxx2 = 0., float dyy2 = 0.):x1(xx1),y1(yy1),x2(xx2),y2(yy2),score(box_score),
    dx1(dxx1),dy1(dyy1),dx2(dxx2),dy2(dyy2){};
    inline deRect GetRect() const {
        int tmp_x1 = round(x1);
        int tmp_y1 = round(y1);
        int tmp_x2 = round(x2);
        int tmp_y2 = round(y2);
        deRect tmp = {tmp_x1, tmp_y1, tmp_x2 - tmp_x1 + 1, tmp_y2 - tmp_y1 + 1};
        return tmp;
    }
    float x1, y1, x2, y2;
    float score;
    float dx1, dy1, dx2, dy2;
    int label;
    float quality;
};

typedef std::vector<Bbox> Vbbox;

//CLASS_NUM = 1:CONF_THRSH=0.55:NMS_THRESH=0.45:INPUT_DIM=416:DET_PERSON_FLAG=0:TINY_YOLO_FLAG=0:ANCHORS=6,11,16,30,27,51,57,81,94,168,143,137,188,180,200,222,224,237
class DE_DLL  YoloV3PostProc {
public:
    YoloV3PostProc(
    const int classes = 80,
    //const int classes = 1,
    // const int input_dimension = 608,
    const int input_dimension = 416,
	//YOLOV3
    const std::vector<int> anchors_box = {10,13,16,30,33,23,30,61,62,45,59,119,116,90,156,198,373,326},
    //const std::vector<int> anchors_box = {6,11,16,30,27,51,57,81,94,168,143,137,188,180,200,222,224,237},
	//YOLOV3TINY
	//const std::vector<int> anchors_box = {10,14,  23,27,  37,58,  81,82,  135,169,  344,319},
    const float conf_thr = 0.55,
    const float nms_thr = 0.45,
    const bool tiny_flag = false,
    const bool square_flag = false,
    //const bool square_flag = true,
    const float extw_scale_face = 0.,
    const float exth_scale_face = 0.);

    ~YoloV3PostProc();

    /**
    * @brief: post proc for yolo nn out
    * @param [in] data: model output, data type: fp16
    * @param [in] data_len: (507 + 2028) * 6 * 2 or (507 + 2028 + 8112) * 6 * 2
    * @param [in] img_w: image pixel width
    * @param [in] img_w: image pixel height 
    * @param [in] eType: yolo model type
    * @retval detect boxes and confidence
    */
    std::vector<std::pair<deRect, float>> run_single_cat(uint8_t* data, int32_t data_len, int32_t img_w, int32_t img_h) const;
    std::vector<de::arctern::MultiCatDetection> run_multi_cat(const std::vector<de::NDArray>& outs, int32_t img_w, int32_t img_h) const;
         

    bool square_face_ = false;
    float exth_scale_face_ = 0.0;
    float extw_scale_face_ = 0.0;

    float *pOutSpace=nullptr;

private:
    void do_nms_sort(detection * dets, int total)const;
    void transform_grid(float* predition)const;
    void ConvertSquare(Vbbox& bbox, float exth_scale, float extw_scale)const;
    Vbbox pack_results(std::vector<detection>& detections, int img_w, int img_h)const;
    std::vector<detection> pack_valid_detections(float* prediction)const;
    Vbbox run(uint8_t* data, int32_t data_len, int img_w, int img_h)const;
    Vbbox run(const std::vector<de::NDArray>& outs, int img_w, int img_h)const;
    Vbbox run3(uint8_t* data1, int32_t data_len1, uint8_t* data2, int32_t data_len2, uint8_t* data3, int32_t data_len3, int img_w, int img_h)const;
    int num_classes;
    int input_dim;
    int nstrides;
    int nboxes;
    std::vector<int> strides;
    double conf_thresh;
    float nms_thresh;
    bool det_person_flag;
    bool tiny_yolov3_flag;
    std::vector<std::vector<int>> anchors;
    std::vector<std::vector<int>> step;

    uint16_t conf_thresh_;
    int32_t conf_thresh_sign;
    int32_t conf_thresh_exponent;
    int32_t conf_thresh_mantissa;
};

}//arctern
}//de

#endif
