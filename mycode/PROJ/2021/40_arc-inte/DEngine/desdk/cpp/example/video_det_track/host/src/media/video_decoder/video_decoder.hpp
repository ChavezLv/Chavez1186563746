/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     video_decoder.h
/// @brief    视频解码模块头文件
/// @author   caoyan
/// @date     2019/8/21
/// @version  0.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef VIDEO_DECODER_HPP
#define VIDEO_DECODER_HPP

#ifdef USE_FFMPEG_DECODER
#include "ffmpeg_decoder.hpp"
#else
#include "gst_decoder.hpp"
#endif
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "if_queue.hpp"
#include "if_log.h"
#include <memory>

namespace intellif {

class VideoDecoder {
  
 public:
  VideoDecoder(IfQueue<SPImage> &frame_queue) : frame_queue_(frame_queue) {
    #ifdef USE_FFMPEG_DECODER
	  ffmpeg_decoder_ = new FFmpegDecoder();
    #else
	  ffmpeg_decoder_ = new GstDecoder();
    #endif
  }
  
  ~VideoDecoder() {
    if (ffmpeg_decoder_) delete ffmpeg_decoder_;
  }

  int open(const std::string &url) {
    DEMO_LOG_INFO("VideoDecoder open url: %s\n", url.c_str());
    #ifdef USE_FFMPEG_DECODER
    FFmpegDecodeCB decode_cb = std::bind(&VideoDecoder::DecodeCallBack, this, std::placeholders::_1);
    #else 
    GstDecodeCB decode_cb = std::bind(&VideoDecoder::DecodeCallBack, this, std::placeholders::_1);
    #endif
    ffmpeg_decoder_->RegisterCallback(decode_cb);
    ffmpeg_decoder_->open(url);

    return 0;
  }
  #if USE_PRIORITY_QUEUE
  void set_queue_in(IfQueueP<SPImage,SPImageCmp> &queue_in) {
  #else
	void set_queue_in(IfQueue<SPImage> &queue_in) {
  #endif
    if (ffmpeg_decoder_) ffmpeg_decoder_->set_queue_in(queue_in);
  }

  void feed(SPImage &image) {
    if (ffmpeg_decoder_) ffmpeg_decoder_->feed(image);
  }

  void DecodeCallBack(SPImage &image) {
    frame_queue_.write(image);
  }

 private:
  IfQueue<SPImage> &frame_queue_;
  #ifdef USE_FFMPEG_DECODER
  FFmpegDecoder *ffmpeg_decoder_ = nullptr;
  #else 
  GstDecoder *ffmpeg_decoder_ = nullptr;
  #endif 
};

#endif //VIDEO_DECODER_HPP

} //namespace intellif


