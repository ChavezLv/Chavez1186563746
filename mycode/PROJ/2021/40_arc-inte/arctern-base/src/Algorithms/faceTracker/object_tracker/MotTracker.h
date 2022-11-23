/// @brief multiple object tracker
/// Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
/// @author Zhang Zhaofeng
/// @date 2018.8.2

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MOTTRACKER_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MOTTRACKER_H_

#include <vector>
#include <map>
#include <string>
#include "opencv2/opencv.hpp"
#include "HungarianAssigning.h"
#include "SingleTracker.h"
#include "SLKTracker.h"
#include "MultiKCFTracker.h"
#include "./overlap.h"
#include "GluonFeature.h"
#include "../define.h"

#define SINGLE_TRACKER_TYPE KCF

enum TrackState { New = 0, Tracked, Lost, Removed, Replaced };
enum SingleState { SOFF = 0, STracked, SLost };

typedef struct {
  int frame_id;
  vector<cv::Rect> rects;
} TestData;

typedef struct {
  cv::Rect rect;
  vector<cv::Rect> rect_history;
  int id;
  bool from_det;
  TrackState state;
  int frame_index;
} TrackInfo;

typedef struct {
  int kf_state_size = 6;
  int kf_measure_size = 4;
  int kf_control_size = 0;
  unsigned int kf_type = CV_32F;
} KalmanFileterParams;

class BaseTracker {
 protected:
  cv::KalmanFilter kf_;
  cv::Mat kf_mean_state_;
  cv::Mat kf_meas_;
  float feat_comb_ratio_ = 0.3;
  int cout_ = 0;
  int frame_id_;
  int track_id_;
  int start_frame_;
  int time_since_update_;
  TrackState state_ = TrackState::New;
  SingleState single_tracking_state_ = SingleState::SOFF;
  vector<float> feature_;
  cv::Rect rect_;
  vector<cv::Rect> rect_history;
  cv::Rect Mat2Rect(cv::Mat stateMat);
  virtual void init_kalman_filter();
  virtual void activate();
  virtual void predict();
  virtual void update();

 public:
  explicit BaseTracker(const KalmanFileterParams &kalman_filter_params);
  ~BaseTracker();
  vector<float> get_feature();
  TrackState get_state();
  SingleState get_single_tracking_state();
  int end_frame();
  void set_feature(vector<float> feature);
  void set_kf_meas_(cv::Rect rect);
  void mark_lost();
  void mark_removed();
  void mark_replaced();
  cv::Rect get_rect();
  vector<cv::Rect> get_rect_history();
  int get_track_id();
};

class STracker : public BaseTracker {
 private:
  KalmanFileterParams kalman_filter_params_;
  int max_n_features_;
  bool is_activated_ = false;
  bool from_det_ = false;
  int tracklet_len_ = 0;
  int time_by_tracking_ = 0;
  float min_approved_overlap_ratio_;
  std::string single_tracker_type_;
  SLKTracker slk_tracker_;
  MultiKCFTracker multi_kcf_tracker_;
  SingleTracker *single_tracker_;
  cv::Mat mean_state_;
  bool use_tracking_;

 public:
  STracker(cv::Rect rect,
           bool from_det,
           std::string single_tracker_type,
           float min_approved_overlap_ratio,
           bool use_tracking,
           const KalmanFileterParams &kalman_filter_params);
  ~STracker();
  bool check_activated();
  bool get_from_det();
  void predict();
  cv::Rect self_tracking(cv::Mat frame_gray, cv::Mat frame_color, int frame_id);
  void init_kalman_filter();
  void init_single_obj_tracker(const cv::Rect &rect,
                               int track_id,
                               const cv::Mat &frame_gray,
                               const cv::Mat &frame_color,
                               float confidence,
                               int detect_plus);
  void reset_single_obj_tracker(const cv::Rect &rect,
                                int track_id,
                                const cv::Mat &frame_gray,
                                const cv::Mat &frame_color,
                                float confidence,
                                int detect_plus);
  void activate(cv::Mat frame_gray, cv::Mat frame_color, int frame_id, int track_id);
  void re_activate(std::shared_ptr<BaseTracker> new_tracker, cv::Mat frame_gray, cv::Mat frame_color, int frame_id);
  void update(std::shared_ptr<BaseTracker> new_trakcer, cv::Mat frame_gray, cv::Mat frame_color, int frame_id);
};

class MotTracker {
 public:
  explicit MotTracker(const arctern::ObjectTrackerParameter &param);
  ~MotTracker();

 private:
  // model parameters
  arctern::ObjectTrackerParameter param_;
  // uint choose_size_;
  // detecor
  int frame_id_ = 0;
  int track_id_ = 0;
  vector<float> variences_;
  vector<std::shared_ptr<STracker>> tracked_stracks_;
  vector<std::shared_ptr<STracker>> lost_stracks_;
  vector<std::shared_ptr<STracker>> removed_stracks_;
  std::map<int, vector<cv::Rect>> detections_load_;

 public:
  arctern::IDetectRecogSink *sink_;

 public:
  // @brief calculate features for eveary rect;
  vector<vector<float>> CalcFeatures(cv::Mat frame_color, vector<cv::Rect> rects);
  // @brief all trackers predict
  void predict(cv::Mat frame_gray, cv::Mat frame_color, int frame_id);
  vector<cv::Rect> Detect(cv::Mat &image, int frame_index);
  // @brief detect object in image, wrap it as a tracker
  vector<std::shared_ptr<STracker>> detect(cv::Mat frame_color, int frame_id = 0);
  // @brief calculate position distance
  void cal_pos_distance(const vector<std::shared_ptr<STracker>> &tracked_stracks,
                        const vector<std::shared_ptr<STracker>> &detection_stracks,
                        double *dists,
                        float gating_thres,
                        vector<float> variences);
  // @brief get output information of every frame
  vector<TrackInfo> get_output();
  vector<TrackInfo> update(cv::Mat frame_gray, cv::Mat frame_color);
  void cal_reid_distance(const vector<std::shared_ptr<STracker>> &tracked_stracks,
                         const vector<std::shared_ptr<STracker>> &detection_stracks,
                         double *dists);
};
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MOTTRACKER_H_
