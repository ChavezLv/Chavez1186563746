
#ifndef MEDIA_DISPLAY_HPP
#define MEDIA_DISPLAY_HPP

#include "video_decoder.hpp"
#include "video_mixer.hpp"
#include "if_common.h"
#include "if_queue.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include <assert.h>

#define TEST_SHOW 0

namespace intellif {

typedef struct{
  int images_num = -1;
  int total_w_size;
  int total_h_size;
  int w;
  int h;
  std::string wnd_name;
}DisplayCfg;

std::mutex s_lock;

class MediaDisplay {

 public:
  MediaDisplay(std::string wnd_name="1", int mode = 0, int fps = 25, int target_size = 480): fps_(fps), decode_queue_(),mix_queue_(),encode_queue_(),data_queue_(),decoder_(mix_queue_), mixer_(mix_queue_, encode_queue_, data_queue_, fps), mode_(mode) {
  
    target_size_ = target_size;
    decoder_.set_queue_in(decode_queue_);
    decoder_.open("");
    wnd_name_ = wnd_name;

    if (s_disp_cfg().images_num < 0){

    }
    else if (s_disp_cfg().images_num == 0){
      cv::namedWindow(wnd_name_);
      std::thread thread(std::bind(&MediaDisplay::display, this));
      thread.detach();
    }else{
      std::unique_lock<std::mutex> locker(lock_);
      static int idx = 0;
      index_ = idx++;
      std::thread thread(std::bind(&MediaDisplay::display2, this));
      thread.detach();
    }
  }
  
  int open(std::string& url){
    return 0;
  }

  int display() {
    printf("enter display thread encode_queue_ %p\n", (void*)&encode_queue_);
    while (1) {
      SPImage image = encode_queue_.read();
      // std::cout<<"encode_queue_ read one image [" <<image->width() <<" "<< image->height()<<"] "<< image->path()<<std::endl;
      float resize_rate = 1;
      if (image->width() > image->height()){
        if (image->height() > target_size_) resize_rate = float(image->height()) / target_size_;
      }else{
        if (image->width() > target_size_) resize_rate = float(image->width()) / target_size_; 
      }

      // cv::Mat mat = cv::imread(image->path());
      cv::Mat mat(image->height(), image->width(), CV_8UC3, image->data());
      cv::Size sz((int)(image->width() / resize_rate), (int)(image->height() / resize_rate));
      cv::Mat bgr_mat = mat;
      if(resize_rate - 1.0 > 0.000001)
        cv::resize(mat, bgr_mat, sz);
      cv::namedWindow(wnd_name_);
      cv::imshow(wnd_name_, bgr_mat);
      cv::waitKey(10);
    }

    return 0;
  }
  
  int display2() {

    auto begin = std::chrono::steady_clock::now();
    int counter=0;
    printf("enter display2 thread index = %d\n",index_);
    intellif::DisplayInfo info;
    while (1) {
      SPImage image = encode_queue_.read();
      // 当前帧没有打框信息，但上一帧有框信息，使用上一帧的框并打框
      if (image->info_.time_stamp == 0 && info.obj_infos.size()>0){
        ImageMixer::mix(image, info);
        info.obj_infos.clear();
      }
      else info = image->info_;
      
      cv::Mat mat(image->height(), image->width(), CV_8UC3, image->data());
      int x = mat.cols;
      int y = mat.rows;
      float scale = (x > y)?( (float)x / s_disp_cfg().total_w_size * s_disp_cfg().w ):((float)y / s_disp_cfg().total_h_size * s_disp_cfg().h);

      int m = (index_ % s_disp_cfg().w) * s_disp_cfg().total_w_size /  s_disp_cfg().w ;
      int n = (index_ / s_disp_cfg().w) * s_disp_cfg().total_h_size /  s_disp_cfg().h ;

      auto start = std::chrono::steady_clock::now();

      cv::Mat imgROI = s_images_mat()(cv::Rect(m, n, (int)(x/scale), (int)(y/scale)));
      {
        //std::unique_lock<std::mutex> locker(s_lock);
        cv::resize(mat, imgROI, cv::Size((int)(x/scale), (int)(y/scale)));
		// cv::imwrite("disp2_1.jpg", imgROI);
      }
      // uint64_t cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
      //printf("disp2 index %2d, cvt cost %ld ms\n",index_, (int64_t)cost);
    }

    return 0;
  }

  void feed_frame(const SPImage & image) {
  //printf("+++MediaDisplay queue_in_ %p size %d\n", (void*)&decode_queue_, decode_queue_.size());
  #if TEST_SHOW
    encode_queue_.write(image);
  #else
    if(mode_ == 0) {
      if (image->format() == DE_PIX_FMT_H264)
        decode_queue_.write(image);
      if (image->format() == IF_IMG_BGR888){
        mix_queue_.write(image);
	  }
	}
    else
		encode_queue_.write(image);
  #endif
  }

  void feed_data(const DisplayInfo & data) {
    data_queue_.write(data);
  }

  static cv::Mat& s_images_mat(){
  static cv::Mat images_mat;
  return images_mat;
  }
  
  static DisplayCfg& s_disp_cfg(){
  static DisplayCfg cfg;
  return cfg;
  }

  static int display3() {
    printf("enter display3\n");
	// cv::namedWindow(s_disp_cfg().wnd_name, CV_WINDOW_NORMAL);
    while (1) {
      cv::namedWindow(s_disp_cfg().wnd_name, CV_WINDOW_NORMAL);
      {
		  //std::unique_lock<std::mutex> locker(s_lock);
		cv::imshow(s_disp_cfg().wnd_name, s_images_mat());
      }
      cv::waitKey(40);
    }

    return 0;
  }
 

  static void config(const std::string wnd_name, int total_w_size, int total_h_size, int nImg){
    s_disp_cfg().wnd_name = wnd_name;
    s_disp_cfg().images_num = nImg;
    s_disp_cfg().total_w_size = total_w_size;
    s_disp_cfg().total_h_size = total_h_size;
    // w - Maximum number of images in a row 
    // h - Maximum number of images in a column 
    int w, h;
    if (nImg <= 0) {
      printf("Number of arguments too small....\n");
      return;
    }
    else if (nImg > 16){
      printf("Number of arguments too large....\n");
      return;
    }
    else if (nImg == 1){
      w = h = 1;
    }
    else if (nImg == 2){
      w = 2; h = 1;;
    }
    else if (nImg == 3 || nImg == 4){
      w = 2; h = 2;
    }
    else if (nImg == 5 || nImg == 6){
      w = 3; h = 2;
    }
    else if (nImg == 7 || nImg == 8){
      w = 4; h = 2;
    }
    else if (nImg <= 12){
      w = 4; h = 3;
    }
    else{
      w = 4; h = 4;
    }
    s_disp_cfg().w = w;
    s_disp_cfg().h = h;
    cv::namedWindow(wnd_name);
    s_images_mat().create(cv::Size(total_w_size, total_h_size), CV_8UC3);
    std::thread thread(std::bind(&MediaDisplay::display3));
    thread.detach();
    printf("config over {%d %d %d %d %d}\n", s_disp_cfg().images_num,s_disp_cfg().total_w_size,s_disp_cfg().total_h_size,
      s_disp_cfg().w,s_disp_cfg().h);
  }
  
 private:
  int target_size_;
  int fps_;
  #if USE_PRIORITY_QUEUE
  IfQueueP<SPImage, SPImageCmp> decode_queue_;
  #else
  IfQueue<SPImage> decode_queue_;
  #endif
  IfQueue<SPImage> mix_queue_;
  IfQueue<SPImage> encode_queue_;
  IfQueue<DisplayInfo> data_queue_; 
 
  VideoDecoder decoder_;
  VideoMixer mixer_;
  std::string wnd_name_;
  std::mutex lock_;
  int index_ = 0;
  int mode_ = 0;  
};

}

#endif //MEDIA_DISPLAY_HPP

