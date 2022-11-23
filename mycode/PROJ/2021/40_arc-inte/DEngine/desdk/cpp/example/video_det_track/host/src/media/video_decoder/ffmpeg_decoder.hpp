/*
 * @file    FFmpegDecoder.h
 * @date    2015-07-08
 * @brief   class defination of Live555RtspGraper
 * @author  zhong.bin (zhong.bin@intellif.com)
 *
 * Copyright (c) 2015, Shenzhen Intellifusion Technologies Co., Ltd.
 */

#ifndef FFMPEG_DECODER_H
#define FFMPEG_DECODER_H

#ifdef _WIN32
#include "Windows.h"
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include "if_common.hpp"
#include "if_log.h"
#include "if_queue.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

extern "C" { 
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/hwcontext.h"
}

#include <functional>
#include <thread>

#define USE_VAAPI 0 ///< 使用VAAPI硬解码
#define USE_FFMPEG_GRAPER 0 ///< 使用FFmpeg取流


namespace intellif {

typedef std::function<void(SPImage &)> FFmpegDecodeCB;

class FFmpegDecoder {

public:

  FFmpegDecoder() {
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
  }
  
  virtual ~FFmpegDecoder() {
    if (fmt_ctx_) avformat_close_input(&fmt_ctx_);
    if (cdc_ctx_) avcodec_free_context(&cdc_ctx_);
  }

  virtual void RegisterCallback(FFmpegDecodeCB decode_cb) {
    DEMO_LOG_INFO("FFmpegDecoder register callback\n");
    decode_cb_ = std::move(decode_cb);
  }

  virtual int open(const std::string &url) {

    fmt_ctx_= avformat_alloc_context();

#if USE_FFMPEG_GRAPER

    AVDictionary *option = nullptr;
    if ((url.find("rtsp://") != std::string::npos) || (url.find("rtmp://") != std::string::npos)) {
      av_dict_set(&option, "rtsp_transport", "tcp", 0); //使用tcp连接
      av_dict_set(&option, "allowed_media_types", "video", 0); //仅解析视频
      //av_dict_set(&option, "hwaccel", "vaapi", 0); //使用vaapi硬件接口
      //av_dict_set(&option, "hwaccel_device", "/dev/dri/renderD128", 0); //使用硬件解码
      //av_dict_set(&avdic, "probesize", "32000000", 0);
    }

    if (int ret = avformat_open_input(&fmt_ctx_, url.c_str(), 0, &option) < 0) {
      DEMO_LOG_ERROR("avformat_open_input fail %d, %s\n", ret, url.c_str());
      return -1;
    }

    for (uint32_t i = 0; i < fmt_ctx_->nb_streams; ++i) {
      if (fmt_ctx_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        stream_id = (int)i;
        break;
      }
    }
    if (-1 == stream_id) {
      avformat_close_input(&fmt_ctx_);
      DEMO_LOG_ERROR("can not find video stream, %s\n", url.c_str());
      return -1;
    }

    avformat_find_stream_info(fmt_ctx_, 0);
    av_dump_format(fmt_ctx_, 0, "", 0);
    
    duration_ = fmt_ctx_->duration;
    start_time_ = fmt_ctx_->start_time;
    time_base_ = av_q2d(fmt_ctx_->streams[stream_id]->time_base);

    DEMO_LOG_INFO("FFmpegDecoder open url success, %s, duration %f, base_time %f, start_time %ld\n", url.c_str(), duration_ * 1.0 / AV_TIME_BASE * 1000, time_base_, start_time_);
    
#endif

#if USE_VAAPI
    AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    cdc_ctx_= avcodec_alloc_context3(codec);
    if (!cdc_ctx_) {
      avformat_close_input(&fmt_ctx_);
      DEMO_LOG_ERROR("avcodec_alloc_context3 fail, %s\n", url.c_str());
      return -1;
    }
    cdc_ctx_->get_format = get_hw_format;
    if (hw_decoder_init(cdc_ctx_, AV_HWDEVICE_TYPE_VAAPI) < 0)
        return -1;

#else	
    AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    cdc_ctx_= avcodec_alloc_context3(codec);

    if (!cdc_ctx_) {
      avformat_close_input(&fmt_ctx_);
      DEMO_LOG_ERROR("avcodec_alloc_context3 fail, %s\n", url.c_str());
      return -1;
    }

#endif

    cdc_ctx_->thread_count = 1;
    int ret = avcodec_open2(cdc_ctx_, codec, nullptr);
    if (ret < 0) {
      DEMO_LOG_ERROR("avcodec_open2 fail, ret = %d %s\n", ret, url.c_str());
      return -1;
    }

    for (int i = 0 ; i < 1; i++){
      std::thread decode_thread(std::bind(&FFmpegDecoder::DecodeLoop, this));
      decode_thread.detach();
    }
    std::thread yuv_thread(std::bind(&FFmpegDecoder::Raw2YUVLoop, this));
    yuv_thread.detach();
    std::thread bgr_thread(std::bind(&FFmpegDecoder::YUV2BGRLoop, this));
    bgr_thread.detach();

    return 0;
  }
  
  void set_queue_in(IfQueue<SPImage> &queue_in) {
    queue_in_ = &queue_in;
  }

  void feed(SPImage &image) {
    queue_in_->write(image);
  }

  virtual void DecodeLoop() {
    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();

    int got_pict= 0;
    
#if USE_FFMPEG_GRAPER
                                     
    while (av_read_frame(fmt_ctx_, pkt) >= 0) {
      if (pkt->stream_index == stream_id) {

#else

    int counter = 0;
    while (1) {
      SPImage image = queue_in_->read();
      pkt->data = image->data();
      pkt->size = image->size();
      //pkt->rtp_ts = image->time_stamp();
      if (1) {

#endif


#if 0 //保存到文件
      static FILE *file = fopen("decode.h264", "w+");
      fwrite(pkt->data, pkt->size, 1, file);
#endif

      struct timeval start_time, end_time;
      gettimeofday(&start_time, NULL);
        
#if USE_VAAPI
        
        int ret = avcodec_send_packet(cdc_ctx_, pkt);

        while (1) {
          ret = avcodec_receive_frame(cdc_ctx_, frame);
          if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
          } else if (ret < 0) {
            DEMO_LOG_ERROR("Error while decoding\n");
            break;
          }
          
          CallBack(frame, image->time_stamp()); //RTP时间戳(编码时间戳)ffmpeg本身不暴露，对ffmpeg修改后暴露用于一流多解时的流间同步
        }        
#else
        int ret = -1;
        {
          std::unique_lock<std::mutex> locker(lock_);
          if (ret = avcodec_decode_video2(cdc_ctx_, frame, &got_pict, pkt) >= 0) {}
        }
        if (ret>=0&&got_pict) CallBack(frame, image->time_stamp());
#endif
        gettimeofday(&end_time, NULL);
        int cost_ms = ((end_time.tv_usec - start_time.tv_usec) / 1000 + (end_time.tv_sec - start_time.tv_sec) * 1000);
        LOG_NOTICE("H264 decode finish, cost %d ms\n", cost_ms);
      }
      av_packet_unref(pkt);
    }

    av_packet_free(&pkt);
    av_frame_free(&frame);

  }

  void CallBack(AVFrame *frame, uint32_t rtp_ts) {
    //DEMO_LOG_DEBUG("FFmpegDecoder, recv frame pts %ld, rtp_ts %u\n", frame->pts, rtp_ts);
    struct timeval start_time, end_time;
    int cost1, cost2, cost3;
    gettimeofday(&start_time, NULL);

    if ((frame->format != AV_PIX_FMT_YUV420P) && (frame->format != AV_PIX_FMT_YUVJ420P) && (frame->format != AV_PIX_FMT_VAAPI)) { 
        DEMO_LOG_ERROR("DecodeCallBack: not supported frame type:%d\n", frame->format);
        assert(0);
        return;
    }
    
    if_image_t image;
    image.time_stamp = rtp_ts;
    image.format = IF_IMG_BGR888;
    image.width = frame->width;
    image.height = frame->height;
    image.size = frame->width * frame->height * 3;
    image.data = nullptr;
    image.extra_data[0] = nullptr;
    image.extra_data[1] = nullptr;
    
    cv::Mat img_mat;
         
    if ((frame->format == AV_PIX_FMT_YUV420P) || (frame->format == AV_PIX_FMT_YUVJ420P)) {
      uint8_t *yuv = new uint8_t[frame->width * frame->height * 3];
      image.format = IF_IMG_YUV420P;
      image.data = frame->data[0];
      image.extra_data[0] = frame->data[1];
      image.extra_data[1] = frame->data[2];
      image_to_yuv(&image, 3 * image.width, yuv);
      img_mat = cv::Mat( image.height, image.width, CV_8UC3, yuv);
      cv::cvtColor(img_mat, img_mat, cv::COLOR_YUV2BGR);
      assert(yuv == img_mat.data); //这里要确认下是否有内存泄漏
	  
    } else if (frame->format == AV_PIX_FMT_VAAPI) {
      /* retrieve data from GPU to CPU */
      static AVFrame *sw_frame = av_frame_alloc();
      if (!sw_frame) {
        DEMO_LOG_ERROR("av_frame_alloc fail!\n");
        return;
      }
      if ((av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
        DEMO_LOG_ERROR("Error transferring the data to system memory\n");
        return;
      }
      gettimeofday(&end_time, NULL);
      cost1 = ((end_time.tv_usec - start_time.tv_usec) / 1000 + (end_time.tv_sec - start_time.tv_sec) * 1000);
      image.format = IF_IMG_NV12;
      image.data = new uint8_t[image.width * image.height];
      if (image.data) memcpy(image.data, sw_frame->data[0], image.width * image.height);
      image.extra_data[0] = new uint8_t[image.width * image.height / 2];
      if (image.extra_data[0]) memcpy(image.extra_data[0], sw_frame->data[1], image.width * image.height / 2);
#if 1 //多线程流水处理，提升性能
      image.size = frame->width * frame->height * 3 / 2;
      SPImage image_ptr(new Image(image));
	    // DEMO_LOG_DEBUG("FFmpegDecoder, raw_queue_ write %d\n", raw_queue_.size());
      raw_queue_.write(image_ptr);
      return;
#endif
      uint8_t *yuv = new uint8_t[frame->width * frame->height * 3];
      image_to_yuv(&image, 3 * frame->width, yuv);
      gettimeofday(&end_time, NULL);
      cost2 = ((end_time.tv_usec - start_time.tv_usec) / 1000 + (end_time.tv_sec - start_time.tv_sec) * 1000);
      img_mat = cv::Mat(image.width, image.height, CV_8UC3, yuv);
      cv::cvtColor(img_mat, img_mat, cv::COLOR_YUV2BGR);
      gettimeofday(&end_time, NULL);
      cost3 = ((end_time.tv_usec - start_time.tv_usec) / 1000 + (end_time.tv_sec - start_time.tv_sec) * 1000);
      LOG_NOTICE("H264 decode cost1 %d ms, cost2 %d ms, cost3 %d ms\n", cost1, cost2, cost3);
      assert(yuv == img_mat.data); //这里要确认下是否有内存泄漏
    } else if (frame->format == AV_PIX_FMT_NV12) {
      //img_mat.create(Size(frame->width, frame->height), CV_8UC3);
      //boost::unique_lock<boost::shared_mutex> writeLocker(IFaceConfiguration::mFMTranLocker);
      //IF_CVT::yuv420_2_rgb_hwc_multhread(frame->data[0], frame->data[1], frame->height, frame->width, img_mat.data);
    } else if (frame->format == AV_PIX_FMT_RGBA) {
      //img_mat.create(Size(frame->width, frame->height), CV_8UC4);
      //Mat mat(frame->height, frame->width, CV_8UC4, frame->data[0], frame->linesize[0]);
      //mat.copyTo(img_mat);
    } else {
      DEMO_LOG_ERROR("receive an wrong YCrCb ImgMat frame->format = %d\n");
      return;
    }
    
    image.format = IF_IMG_BGR888;
    image.data = img_mat.data;
    image.extra_data[0] = NULL;
    image.extra_data[1] = NULL;
    SPImage image_ptr(new Image(image));
    //DEMO_LOG_DEBUG("DecodeCallBack image format %d, ts %f, width %d, height %d\n", frame->format, frame->pts * time_base_ * 1000, frame->width, frame->height);

    if (decode_cb_) decode_cb_(image_ptr);
  }
  
  void Raw2YUVLoop() {
    while (1) {
      SPImage frame = raw_queue_.read();
	  DEMO_LOG_DEBUG("Raw2YUVLoop got one\n");
      uint8_t *img_data = new uint8_t[frame->width() * frame->height() * 3];
      image_to_yuv(&(frame->image()), 3 * frame->width(), img_data);
      if (frame->image().data) {
        delete[] frame->image().data;
        frame->image().data = img_data;
      }
      if (frame->image().extra_data[0]) {
        delete[] frame->image().extra_data[0];
        frame->image().extra_data[0] = NULL;
      }
      if (frame->image().extra_data[1]) {
        delete[] frame->image().extra_data[1];
        frame->image().extra_data[1] = NULL;
      }
      frame->image().format = IF_IMG_YUV;
      frame->image().size = frame->width() * frame->height() * 3;
	  

	    //DEMO_LOG_DEBUG("Raw2YUVLoop, yuv_queue_ write %d\n", yuv_queue_.size());
      yuv_queue_.write(frame);
    }
  }
  
  void YUV2BGRLoop() {
    while (1) {
      SPImage frame = yuv_queue_.read();
	    DEMO_LOG_DEBUG("YUV2BGRLoop got one\n");
      cv::Mat img_mat(frame->width(), frame->height(), CV_8UC3, frame->data());
      cv::cvtColor(img_mat, img_mat, cv::COLOR_YUV2BGR);
      assert(frame->data() == img_mat.data); //这里要确认下是否有内存泄漏
      frame->image().format = IF_IMG_BGR888;
	    DEMO_LOG_DEBUG("YUV2BGRLoop decode_cb_\n");
      if (decode_cb_) decode_cb_(frame);
	    DEMO_LOG_DEBUG("YUV2BGRLoop decode_cb_ ok\n");
    }
  }

 private:

  FFmpegDecodeCB decode_cb_;
  IfQueue<SPImage> internal_queue_;
  IfQueue<SPImage> *queue_in_ = &internal_queue_;  
  IfQueue<SPImage> raw_queue_, yuv_queue_;

  std::mutex lock_;

  int stream_id = -1;

  AVFormatContext *fmt_ctx_ = nullptr;
  AVCodecContext *cdc_ctx_ = nullptr;
  AVBufferRef *hw_device_ctx_ = nullptr;
  AVCodecParserContext *parser_ = nullptr;

  int64_t duration_ = 0;
  int64_t start_time_ = 0;
  double time_base_ = 0.0;

  int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type)
  {
    int err = 0;

    if ((err = av_hwdevice_ctx_create(&hw_device_ctx_, type, NULL, NULL, 0)) < 0) {
        DEMO_LOG_ERROR("Failed to create specified HW device.\n");
        return err;
    }
    ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx_);

    return err;
  }

  static enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts)
  {
    return AV_PIX_FMT_VAAPI;
  }

  void image_to_yuv(if_image_t *image, int widthStep, uint8_t *outYuv) {
    uint8_t *pcOutWalk0 = outYuv;
    uint8_t *pcOutWalk1 = outYuv + widthStep;
    int    dwYIdx0    = 0;
    int    dwYIdx1    = image->width;
    int    dwUVIdx    = 0;

    if (image->format == IF_IMG_YUV420P) {
      for (int dwRowIdx = 0; dwRowIdx  < image->height; dwRowIdx += 2) {
        for (int dwColIdx = 0; dwColIdx < image->width; dwColIdx += 4) {
          int dwNextUVIdx = dwUVIdx + 1;
          
          pcOutWalk0[0]  = image->data[dwYIdx0];
          pcOutWalk0[1]  = image->extra_data[0][dwUVIdx];
          pcOutWalk0[2]  = image->extra_data[1][dwUVIdx];
          pcOutWalk0[3]  = image->data[dwYIdx0 + 1];
          pcOutWalk0[4]  = image->extra_data[0][dwUVIdx];
          pcOutWalk0[5]  = image->extra_data[1][dwUVIdx];

          pcOutWalk0[6]  = image->data[dwYIdx0 + 2];
          pcOutWalk0[7]  = image->extra_data[0][dwNextUVIdx];
          pcOutWalk0[8]  = image->extra_data[1][dwNextUVIdx];
          pcOutWalk0[9]  = image->data[dwYIdx0 + 3];
          pcOutWalk0[10] = image->extra_data[0][dwNextUVIdx];
          pcOutWalk0[11] = image->extra_data[1][dwNextUVIdx];
          
          pcOutWalk1[0]  = image->data[dwYIdx1];
          pcOutWalk1[1]  = image->extra_data[0][dwUVIdx];
          pcOutWalk1[2]  = image->extra_data[1][dwUVIdx];
          pcOutWalk1[3]  = image->data[dwYIdx1 + 1];
          pcOutWalk1[4]  = image->extra_data[0][dwUVIdx];
          pcOutWalk1[5]  = image->extra_data[1][dwUVIdx];

          pcOutWalk1[6]  = image->data[dwYIdx1 + 2];
          pcOutWalk1[7]  = image->extra_data[0][dwNextUVIdx];
          pcOutWalk1[8]  = image->extra_data[1][dwNextUVIdx];
          pcOutWalk1[9]  = image->data[dwYIdx1 + 3];
          pcOutWalk1[10] = image->extra_data[0][dwNextUVIdx];
          pcOutWalk1[11] = image->extra_data[1][dwNextUVIdx];

          pcOutWalk0 += 12;
          pcOutWalk1 += 12;

          dwYIdx0    += 4;
          dwYIdx1    += 4;

          dwUVIdx    += 2;
        }

        dwYIdx0    += image->width;
        dwYIdx1    += image->width;

        pcOutWalk0 += widthStep;
        pcOutWalk1 += widthStep;
      }
    } else if (image->format == IF_IMG_NV12) {
      for (int dwRowIdx = 0; dwRowIdx  < image->height; dwRowIdx += 2) {
        for (int dwColIdx = 0; dwColIdx < image->width; dwColIdx += 4) {
          pcOutWalk0[0]  = image->data[dwYIdx0];
          pcOutWalk0[1]  = image->extra_data[0][dwUVIdx];
          pcOutWalk0[2]  = image->extra_data[0][dwUVIdx + 1];
          pcOutWalk0[3]  = image->data[dwYIdx0 + 1];
          pcOutWalk0[4]  = image->extra_data[0][dwUVIdx];
          pcOutWalk0[5]  = image->extra_data[0][dwUVIdx + 1];

          pcOutWalk0[6]  = image->data[dwYIdx0 + 2];
          pcOutWalk0[7]  = image->extra_data[0][dwUVIdx + 2];
          pcOutWalk0[8]  = image->extra_data[0][dwUVIdx + 3];
          pcOutWalk0[9]  = image->data[dwYIdx0 + 3];
          pcOutWalk0[10] = image->extra_data[0][dwUVIdx + 2];
          pcOutWalk0[11] = image->extra_data[0][dwUVIdx + 3];

          pcOutWalk1[0]  = image->data[dwYIdx1];
          pcOutWalk1[1]  = image->extra_data[0][dwUVIdx];
          pcOutWalk1[2]  = image->extra_data[0][dwUVIdx + 1];
          pcOutWalk1[3]  = image->data[dwYIdx1 + 1];
          pcOutWalk1[4]  = image->extra_data[0][dwUVIdx];
          pcOutWalk1[5]  = image->extra_data[0][dwUVIdx + 1];

          pcOutWalk1[6]  = image->data[dwYIdx1 + 2];
          pcOutWalk1[7]  = image->extra_data[0][dwUVIdx + 2];
          pcOutWalk1[8]  = image->extra_data[0][dwUVIdx + 3];
          pcOutWalk1[9]  = image->data[dwYIdx1 + 3];
          pcOutWalk1[10] = image->extra_data[0][dwUVIdx + 2];
          pcOutWalk1[11] = image->extra_data[0][dwUVIdx + 3];

          pcOutWalk0 += 12;
          pcOutWalk1 += 12;

          dwYIdx0    += 4;
          dwYIdx1    += 4;

          dwUVIdx    += 4;
        }

        dwYIdx0    += image->width;
        dwYIdx1    += image->width;

        pcOutWalk0 += widthStep;
        pcOutWalk1 += widthStep;
      }
    } else {
      DEMO_LOG_ERROR("AVFrame2YUV unsupport format: %d\n", image->format);
    }
  }
  
};

} //namespace intellif

#endif //FFMPEG_DECODER_H

