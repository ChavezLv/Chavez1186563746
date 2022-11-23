/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     image_mixer.h
/// @brief    图像叠加头文件
/// @author   caoyan
/// @date     2019/8/21
/// @version  0.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_MIXER_H
#define IMAGE_MIXER_H


#include "media_common.h"
#include "if_common.hpp"
#include "if_log.h"
#include "de_arctern_node.h"

namespace intellif {

class ImageMixer {
  
 public:

  static int mix(SPImage &image, const DisplayInfo &info) {
    if (image->format() == IF_IMG_BGR888) {
      cv::Mat image_mat(image->height(), image->width(), CV_8UC3, image->data());
	  cv::Scalar color(122, 0, 122);
      cv::Scalar color_b(255, 0, 0);
	  cv::Scalar color_g(0, 255, 0);
	  cv::Scalar color_r(0, 0, 255);
      
      //画框并显示识别信息
      for (auto iter = info.obj_infos.begin(); iter != info.obj_infos.end(); iter++)
      {
        if (iter->color == de::arctern::FACE_CAT_ID)
			color = color_b;
		else if (iter->color == de::arctern::HUMAN_CAT_ID)
			color = color_g;
		else if (iter->color == de::arctern::CAR_CAT_ID)
			color = color_r;
		else if (iter->color == de::arctern::BIKE_CAT_ID)
			color = color;
		
        int w = iter->rect.right - iter->rect.left + 1;
        int h = iter->rect.bottom - iter->rect.top + 1;
        
#if 1 //画框
        cv::rectangle(image_mat, cv::Rect(iter->rect.left, iter->rect.top, w, h), color, 3);
        // if (!iter->top_text.empty())
        // cv::putText(image_mat, iter->top_text, cv::Point(iter->rect.left, iter->rect.top - 5), 
            // CV_FONT_HERSHEY_SIMPLEX, 2, color, 2);
		if (!iter->side_text.empty())
        cv::putText(image_mat, iter->side_text, cv::Point(iter->rect.right + 5, iter->rect.top + 15), 
            CV_FONT_HERSHEY_SIMPLEX, 2, color, 2);
		if (!iter->bottom_text.empty())
        cv::putText(image_mat, iter->bottom_text, cv::Point(iter->rect.left, iter->rect.bottom + 15), 
            CV_FONT_HERSHEY_SIMPLEX, 2, color, 2);

#else //打logo

        int roi_left = iter->rect.left + w / 2 - logo_.cols / 2;
        int roi_top = iter->rect.top + h / 2 - logo_.rows / 2;
        int roi_right = iter->rect.left + w / 2 + logo_.cols / 2 - 1;
        int roi_bottom = iter->rect.top + h / 2 + logo_.rows / 2 - 1;
        roi_left = MIN(MAX(roi_left, 0), image_mat.cols - 1);
        roi_top = MIN(MAX(roi_top, 0), image_mat.rows - 1);
        roi_right = MIN(MAX(roi_right, 0), image_mat.cols - 1);
        roi_bottom = MIN(MAX(roi_bottom, 0), image_mat.rows - 1);
        cv::Mat roi = image_mat(cv::Rect(roi_left, roi_top, roi_right - roi_left + 1, roi_bottom - roi_top + 1));
        logo_.copyTo(roi);
        roiw = roi_right - roi_left + 1;
        roih = roi_bottom - roi_top + 1;
#endif
       
      }
	  //端到端延时显示
      cv::putText(image_mat, "detdelay:"+std::to_string(info.det_delay), cv::Point(image->width()/10, image->height()/10), 
		CV_FONT_HERSHEY_SIMPLEX, 1, color_r, 2);
  
		// cv::putText(image_mat, "eachdelay:"+std::to_string(info.trk_delay), cv::Point(image->width()/2, image->height()/10), 
		// CV_FONT_HERSHEY_SIMPLEX, 2, color_r, 2);
    } else {
      DEMO_LOG_ERROR("format %d not supported!", image->format());
      return -1;
    }

    return 0;
  }

  static cv::Mat logo_;
  
 private:
};

} //namespace intellif

#endif //IMAGE_MIXER_H

