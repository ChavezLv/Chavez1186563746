/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MOT_PARAMETER_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MOT_PARAMETER_H_
#include <stdio.h>
#include <string>
namespace motparameter {
typedef struct {
  std::string video_name;
  std::string detect_list;
  std::string dest_image_dir;
  std::string dest_list_dir;
} IOParams;

typedef struct {
  std::string model_path = "/home/zhangzhaofeng/face/face_selecting/model/mtcnn/v0.9.0";
  int min_size = 32;
  float pnet_thresh = 0.75;
  float rnet_thresh = 0.7;
  float onet_thresh = 0.97;
  float factor = 0.709;
  bool profile = false;
  bool square = true;
  float exth_scale = 0.0;
  float extw_scale = 0.0;
  int mostN = -1;
} MtcnnParams;

typedef struct {
  std::string model_json = "/home/zhangzhaofeng/face/face_selecting/model/prophet_model/v2/prophet-symbol.json";
  std::string model_params = "/home/zhangzhaofeng/face/face_selecting/model/prophet_model/v2/prophet-0000.params";
  int height = 112;
  int width = 96;
  int batch_size = 1;
} ProphetParams;

typedef struct {
  std::string model_path = "/home/zhangzhaofeng/face/feature/v2.0";
  int height = 112;
  int width = 96;
  int batch_size = 1;
  float exth_scale = 0.083;
  float extw_scale = 0.0;
} GluonParams;

typedef struct {
  bool load_rect_flag = true;
  uint choose_size = 32;
} DetectParams;

typedef struct {
  int detect_period = 1;  // do 1 face detecting per detect_period
  int detect_id = 0;  // if frame_num%detect_period == detect_id, then detect
  double min_ap_dist = 0.4;  // min match similarity
  float min_approved_overlap_ratio = 0.0;  // min overlap ratio between tracked rect and kalman predicted rect.
  int max_time_lost = 36;
  bool use_tracking = false;  // use tracking on no detected frames
  std::string single_tracker_type;  // "KCF" or "SLK"
  bool use_refind = true;  // use refind if track object lost
  float gating_thres;  // distance threshold between matched rect and predicted rect
} TrackParams;

typedef struct {
  int snap_interval = 25;  // do 1 snap in 25 frames
  bool feature_flip_flag = true;  // flip image and combine feature
} SnapParams;

typedef struct {
  int gpu = 25;  // gpu index
} HardWareParams;
}  // namespace motparameter
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MOT_PARAMETER_H_
