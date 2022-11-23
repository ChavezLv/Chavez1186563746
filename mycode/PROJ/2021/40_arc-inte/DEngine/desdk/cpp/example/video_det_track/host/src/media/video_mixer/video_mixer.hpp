/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     video_mixer.hpp
/// @brief    视频叠加头文件
/// @author   caoyan
/// @date     2019/8/21
/// @version  0.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef VIDEO_MIXER_HPP
#define VIDEO_MIXER_HPP


#include "image_mixer.hpp"
#include "media_common.h"
#include "media_session.h"
#include "if_common.hpp"

#define ABS(x) (x >= 0 ? x : -x)

namespace intellif {

class VideoMixer {
  
 public:
  VideoMixer(IfQueue<SPImage> &inqueue, IfQueue<SPImage> &outqueue, IfQueue<DisplayInfo> &data_queue, int max_frame_num = 8) 
    : inqueue_(inqueue), outqueue_(outqueue), data_queue_(data_queue), max_frame_num_(max_frame_num) {  
#if USE_UCLIBC
    pthread_t tid;
    pthread_create(&tid, NULL, VideoMixer::recv_frame_run, this);
    pthread_t tid;
    pthread_create(&tid, NULL, VideoMixer::recv_data_run, this);
#else
    std::thread recv_frame_thread(std::bind(&VideoMixer::recv_frame_loop, this));
    recv_frame_thread.detach();
    std::thread recv_data_thread(std::bind(&VideoMixer::recv_data_loop, this));
    recv_data_thread.detach();
#endif
  }
  
  ~VideoMixer() {}

#if USE_UCLIBC
  static void* recv_frame_run(void *thiz) {
    pthread_detach(pthread_self());
    VideoMixer *p = (VideoMixer *)thiz;
    p->recv_frame_loop();
  }

  static void* recv_data_run(void *thiz) {
    pthread_detach(pthread_self());
    VideoMixer *p = (VideoMixer *)thiz;
    p->recv_data_loop();
  }
#endif
  
  void recv_frame_loop() {
    while (1) {
      SPImage frame_in = inqueue_.read();
	    DEMO_LOG_DEBUG("VideoMixer got one frame\n");
      mtx_frame_queue_.lock();
      if (frame_queue_.size() >= max_frame_num_) {
        SPImage frame_out = frame_queue_.front();
        frame_queue_.pop_front();
        outqueue_.write(frame_out);
		  //DEMO_LOG_DEBUG("VideoMixer outqueue_ %p write size %d \n", (void*)&outqueue_, outqueue_.size());
        is_send_ = true;
      }
	    DEMO_LOG_DEBUG("VideoMixer frame_queue_ write  size %d \n",frame_queue_.size());
      frame_queue_.emplace_back(frame_in);
      mtx_frame_queue_.unlock();
    }
  }

  void recv_data_loop() {
	uint32_t det_period = 8;
	uint32_t det_counter=0;
	uint32_t trk_counter=0;
	uint64_t det_delay_sum=0;
	uint64_t trk_delay_sum=0;
	uint64_t det_delay_avg=0;
	uint64_t trk_delay_avg=0;
    while (1) {
      DisplayInfo info = data_queue_.read();
	  
	  if(info.frame_id%det_period == 0)
		  det_delay_avg = info.trk_delay;
	  info.det_delay = det_delay_avg;
	  //端到端延时
	  // timespec time1;
	  // clock_gettime(CLOCK_MONOTONIC,&time1);
	  // uint64_t delay = time1.tv_sec*1000000000 + time1.tv_nsec - info.time_stamp; 
	  // if(info.frame_id%det_period == 0){
		// det_counter++;
		// det_delay_sum+=delay;
	  // }
	  // else{
		// trk_counter++;
		// trk_delay_sum+=delay;
	  // }
	  // if (det_counter == 3){
		// det_delay_avg = det_delay_sum/det_counter;	
		// trk_delay_avg = trk_delay_sum/trk_counter;
		// det_counter = 0;
		// trk_counter = 0;
		// det_delay_sum = 0;
		// trk_delay_sum = 0;
	  // }
	  // info.det_delay = det_delay_avg;	
	  // info.trk_delay = trk_delay_avg;
	  
	  //框入队列，取队列第一个框匹配视频帧打框，打框成功则出列，否则等待。
      disp_info_.push_back(info);
      DisplayInfo data = disp_info_.front();
      mtx_frame_queue_.lock();
      bool is_found = false;
      uint64_t latest_ts = 0;
      int64_t cur_diff = 0;
      int64_t diff = 0;
	  // timespec time1;
	  // clock_gettime(CLOCK_MONOTONIC,&time1);//CLOCK_REALTIME CLOCK_MONOTONIC
	  // uint64_t curtime = time1.tv_sec*1000000000 + time1.tv_nsec;
	  // uint64_t delta = curtime - data.time_stamp;
      // DEMO_LOG_INFO("receive data %ld, curtime %ld, delta=%ld frame_queue_size %d\n", data.time_stamp, curtime,delta, frame_queue_.size());
      if (frame_queue_.size()) {
		//帧队列最新帧的时间戳
        latest_ts = frame_queue_.back()->time_stamp();
        cur_diff = (latest_ts - data.time_stamp);
        if (cur_diff >= 0) {
          for (auto iter = frame_queue_.begin(); iter != frame_queue_.end(); iter++) {
            SPImage image = *iter;
            diff = (image->time_stamp() - data.time_stamp);
            if (ABS(diff) <= 100) { //允许时间戳差为100
              			  
              ImageMixer::mix(image, data);
              image->info_ = data;
              LOG_NOTICE("image mix ts %ld, info ts %ld, diff %ld, cur_diff %ld, obj_num %d\n", image->time_stamp(), 
                data.time_stamp, diff, cur_diff, data.obj_infos.size());
              is_found = true;
              break;
            }
          }
          disp_info_.pop_front();
        }
        else {
          DEMO_LOG_ERROR("display info mismatch! cur_diff %d, info_size  %d\n", cur_diff, disp_info_.size());
        }
      }
      if (!is_found) {
        DEMO_LOG_ERROR("display info mismatch! info ts %ld, latest_ts %ld, queue size %d, cur_diff %ld\n", data.time_stamp, latest_ts, frame_queue_.size(), cur_diff);
      }
      mtx_frame_queue_.unlock();
    }
  }

  void send_loop() { //视频停止后将缓冲区内剩下的帧推出，暂时不用
    while (1) {
      mtx_frame_queue_.lock();
      if (!is_send_) {
        //SPIFMat frame = frame_queue_.front();
        frame_queue_.pop_front();
        //outqueue_.write(frame);
        is_send_ = true;
      }
      mtx_frame_queue_.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
  }

 private:
  std::string id_;
  std::list<DisplayInfo> disp_info_;
  std::list<SPImage> frame_queue_;
  std::mutex mtx_frame_queue_;
  IfQueue<SPImage> &inqueue_;
  IfQueue<SPImage> &outqueue_;
  IfQueue<DisplayInfo> &data_queue_;
  bool is_send_ = false;
  int max_frame_num_ = 8;
};

} //namespace intellif

#endif //VIDEO_MIXER_HPP

