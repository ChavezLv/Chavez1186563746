/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceTracker/object_tracker/MotTracker.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "common/encryx.h"

using std::map;
using cv::Rect;
using cv::Mat;
using std::shared_ptr;

map<int, vector<Rect>> g_test_data = {
    {0, {{1106, 126, 57, 57}}},
    {25, {{1136, 143, 55, 55}, {1243, 140, 49, 49}}},
    {50, {{977, 154, 60, 60}, {1232, 139, 52, 52}}},
    {75, {{793, 178, 82, 82}, {748, 129, 49, 49}, {1239, 137, 54, 54}, {748, 129, 49, 49}}},
    {100, {{769, 206, 107, 107}, {717, 105, 61, 61}, {843, 150, 58, 58}, {717, 105, 61, 61}}},
    {125, {{1103, 133, 51, 51}, {891, 175, 62, 62}, {714, 335, 125, 125}, {993, 188, 53, 53}, {805, 128, 56, 56},
           {1242, 139, 50, 50}, {845, 121, 50, 50}, {721, 130, 60, 60}}}
};

// mapDetects g_map_detect;

BaseTracker::BaseTracker(const KalmanFileterParams &kalman_filter_params) :
    kf_(kalman_filter_params.kf_state_size,
        kalman_filter_params.kf_measure_size,
        kalman_filter_params.kf_control_size,
        CV_32F),
    kf_mean_state_(kalman_filter_params.kf_state_size, 1, kalman_filter_params.kf_type),
    kf_meas_(kalman_filter_params.kf_measure_size, 1, kalman_filter_params.kf_type) {
}

BaseTracker::~BaseTracker() {
}

int BaseTracker::end_frame() {
  return frame_id_;
}

void BaseTracker::init_kalman_filter() {
}
void BaseTracker::activate() {
}
void BaseTracker::predict() {
}
void BaseTracker::update() {
}

Rect BaseTracker::Mat2Rect(Mat stateMat) {
  cv::Rect rect;
  rect.width = stateMat.at<float>(4);
  rect.height = stateMat.at<float>(5);
  rect.x = stateMat.at<float>(0) - rect.width / 2;
  rect.y = stateMat.at<float>(1) - rect.height / 2;
  return rect;
}

void BaseTracker::mark_lost() {
  state_ = TrackState::Lost;
  single_tracking_state_ = SingleState::SLost;
}

void BaseTracker::mark_removed() {
  state_ = TrackState::Removed;
}

void BaseTracker::mark_replaced() {
  state_ = TrackState::Replaced;
}
void BaseTracker::set_feature(vector<float> feature) {
  if (feature_.size() == 0) {
    feature_ = feature;
  } else {
    feature_ = CombineFeature(feature, feature_, feat_comb_ratio_);
  }
}

void BaseTracker::set_kf_meas_(Rect rect) {
  kf_meas_.at<float>(0) = rect.x;
  kf_meas_.at<float>(1) = rect.y;
  kf_meas_.at<float>(2) = rect.width;
  kf_meas_.at<float>(3) = rect.height;
}

vector<float> BaseTracker::get_feature() {
  return feature_;
}

Rect BaseTracker::get_rect() {
  return rect_;
}

vector<Rect> BaseTracker::get_rect_history() {
  return rect_history;
}

TrackState BaseTracker::get_state() {
  return state_;
}

SingleState BaseTracker::get_single_tracking_state() {
  return single_tracking_state_;
}

int BaseTracker::get_track_id() {
  return track_id_;
}

STracker::STracker(Rect rect,
                   bool from_det,
                   std::string single_tracker_type,
                   float min_approved_overlap_ratio,
                   bool use_tracking,
                   const KalmanFileterParams &kalman_filter_params) :
    BaseTracker(kalman_filter_params),
    kalman_filter_params_(kalman_filter_params),
    slk_tracker_(0, 1, 1),
    multi_kcf_tracker_(MultiKCFTracker()) {
  rect_ = rect;
  rect_history.push_back(rect_);
  from_det_ = from_det;
  min_approved_overlap_ratio_ = min_approved_overlap_ratio;
  single_tracker_type_ = single_tracker_type;
  use_tracking_ = use_tracking;
}

STracker::~STracker() {
}

bool STracker::check_activated() {
  return is_activated_;
}

bool STracker::get_from_det() {
  return from_det_;
}

void STracker::predict() {
  if (time_since_update_ > 0)
    tracklet_len_ = 0;
  time_since_update_ += 1;
  kf_mean_state_ = kf_.predict();
  Rect kf_rect = Mat2Rect(kf_mean_state_);
  rect_ = kf_rect;
}

Rect STracker::self_tracking(Mat frame_gray, Mat frame_color, int frame_id) {
  Rect kf_rect = Mat2Rect(kf_mean_state_);
  if (single_tracking_state_ == SingleState::STracked) {
    single_tracker_->Process(frame_gray, frame_color, frame_id);
    Rect *cur_pos = single_tracker_->GetCurPos();
    double score = 0.0;
    if (cur_pos != NULL) {
      seRectPair M = seRectPair(*cur_pos, kf_rect);
      score = M.getOverlapRatio();
    } else {
      score = -1.0;
    }
    if (score > min_approved_overlap_ratio_) {
      rect_ = *cur_pos;
      single_tracking_state_ = SingleState::STracked;
      // if single object tracking valid, then correct kalman filter
      set_kf_meas_(rect_);
      kf_.correct(kf_meas_);
    } else {
      single_tracking_state_ = SingleState::SLost;
    }
  }
  return rect_;
}

void STracker::init_kalman_filter() {
  cv::setIdentity(kf_.transitionMatrix);
  kf_.transitionMatrix.at<float>(2) = 1.0;
  kf_.transitionMatrix.at<float>(9) = 1.0;
  kf_.measurementMatrix = cv::Mat::zeros(kalman_filter_params_.kf_measure_size,
                                         kalman_filter_params_.kf_state_size,
                                         kalman_filter_params_.kf_type);
  kf_.measurementMatrix.at<float>(0) = 1.0f;
  kf_.measurementMatrix.at<float>(7) = 1.0f;
  kf_.measurementMatrix.at<float>(16) = 1.0f;
  kf_.measurementMatrix.at<float>(23) = 1.0f;
  kf_.processNoiseCov.at<float>(0) = 0.01f;
  kf_.processNoiseCov.at<float>(7) = 0.01f;
  kf_.processNoiseCov.at<float>(14) = 1.0f;
  kf_.processNoiseCov.at<float>(21) = 1.0f;
  kf_.processNoiseCov.at<float>(28) = 1.0f;
  kf_.processNoiseCov.at<float>(35) = 1.0f;
  cv::setIdentity(kf_.measurementNoiseCov, cv::Scalar(1e-1));
  kf_.measurementNoiseCov.at<float>(0) = 5.0f;
  kf_.measurementNoiseCov.at<float>(5) = 5.0f;
  kf_.measurementNoiseCov.at<float>(10) = 3.0f;
  kf_.measurementNoiseCov.at<float>(15) = 3.0f;
  kf_.errorCovPre.at<float>(0) = 1;
  kf_.errorCovPre.at<float>(7) = 1;
  kf_.errorCovPre.at<float>(14) = 1;
  kf_.errorCovPre.at<float>(21) = 1;
  kf_.errorCovPre.at<float>(28) = 1;
  kf_.errorCovPre.at<float>(35) = 1;
  kf_mean_state_.at<float>(0) = rect_.x;
  kf_mean_state_.at<float>(1) = rect_.y;
  kf_mean_state_.at<float>(2) = 0;
  kf_mean_state_.at<float>(3) = 0;
  kf_mean_state_.at<float>(4) = rect_.width;
  kf_mean_state_.at<float>(5) = rect_.height;
  kf_.statePost = kf_mean_state_;
}

void STracker::init_single_obj_tracker(const Rect &rect,
                                       int track_id,
                                       const Mat &frame_gray,
                                       const Mat &frame_color,
                                       float confidence,
                                       int detect_plus) {
  if (single_tracker_type_ == "SLK") {
    single_tracker_ = &slk_tracker_;
    single_tracker_->ResetTracker(rect, track_id, frame_gray, confidence, 0);
  }
  if (single_tracker_type_ == "KCF") {
    single_tracker_ = &multi_kcf_tracker_;
    single_tracker_->ResetTracker(rect, track_id, frame_color, confidence, 0);
  }
  // confidence = 1.0;//new added face
  single_tracker_->InitCandiFrameRect();
  single_tracking_state_ = SingleState::STracked;
}

void STracker::reset_single_obj_tracker(const Rect &rect,
                                        int track_id,
                                        const Mat &frame_gray,
                                        const Mat &frame_color,
                                        float confidence,
                                        int detect_plus) {
  if (single_tracker_type_ == "SLK") {
    single_tracker_->ResetTracker(rect, track_id, frame_gray, confidence, 0);
  }
  if (single_tracker_type_ == "KCF") {
    single_tracker_->ResetTracker(rect, track_id, frame_color, confidence, 0);
  }
  single_tracking_state_ = SingleState::STracked;
}

void STracker::activate(Mat frame_gray, Mat frame_color, int frame_id, int track_id) {
  track_id_ = track_id;
  // init kalman filter
  init_kalman_filter();
  // init single object tracker
  if (use_tracking_ == true) {
    float confidence = 1.0;  // new added face
    init_single_obj_tracker(rect_, track_id, frame_gray, frame_color, confidence, 0);
  }
  time_since_update_ = 0;
  time_by_tracking_ = 0;
  tracklet_len_ = 0;
  state_ = TrackState::Tracked;
  frame_id_ = frame_id;
  start_frame_ = frame_id;
}

void STracker::re_activate(std::shared_ptr<BaseTracker> new_tracker, Mat frame_gray, Mat frame_color, int frame_id) {
  rect_ = new_tracker->get_rect();
  set_kf_meas_(new_tracker->get_rect());
  kf_.correct(kf_meas_);
  // reset single object tracker
  if (use_tracking_ == true) {
    float confidence = 1.0;
    reset_single_obj_tracker(rect_, track_id_, frame_gray, frame_color, confidence, 0);
  }
  time_since_update_ = 0;
  time_by_tracking_ = 0;
  tracklet_len_ = 0;
  state_ = TrackState::Tracked;
  frame_id_ = frame_id;
  set_feature(new_tracker->get_feature());
}

void STracker::update(std::shared_ptr<BaseTracker> new_tracker, Mat frame_gray, Mat frame_color, int frame_id) {
  rect_ = new_tracker->get_rect();
  rect_history.push_back(rect_);
  set_kf_meas_(new_tracker->get_rect());
  kf_.correct(kf_meas_);
  time_since_update_ = 0;
  time_by_tracking_ = 0;
  tracklet_len_ += 1;
  state_ = TrackState::Tracked;
  frame_id_ = frame_id;
  set_feature(new_tracker->get_feature());
  is_activated_ = true;
  if (use_tracking_ == false) {
    return;
  }
  float confidence = 1.0;  // new added face
  if (single_tracker_type_ == "SLK") {
    single_tracker_->ResetTracker(new_tracker->get_rect(), track_id_, frame_gray, confidence, 0);
  }
  if (single_tracker_type_ == "KCF") {
    single_tracker_->ResetTracker(new_tracker->get_rect(), track_id_, frame_color, confidence, 0);
  }
}

MotTracker::MotTracker(const arctern::ObjectTrackerParameter &param)
    : param_(param) {
  track_id_ = 0;
  variences_.push_back(1.0);
  variences_.push_back(1.0);
  variences_.push_back(1.0);
  variences_.push_back(1.0);
  sink_ = nullptr;
  // g_map_detect = parseMotDetectLog("/home/tianqinjing/gallen_folder/arctern/data/data.lst");
  // cout << "0000000000000000000000000000000000000000000000000000" << endl;
  // cout << g_map_detect.size() << endl;
  // cout << "0000000000000000000000000000000000000000000000000000" << endl;
}

MotTracker::~MotTracker() {}

vector<Rect> MotTracker::Detect(Mat &frame_color, int frame_index) {
  if (sink_) {
    auto rects = sink_->OnDetect(frame_color);
    std::vector<cv::Rect> detections;
    for (auto rect : rects) {
      validate_rect(&rect, frame_color.cols, frame_color.rows);
      if ((rect.height < param_.choose_size) || (rect.width < param_.choose_size))
        continue;
      detections.push_back(rect);
    }
    return detections;
  } else {
    std::cout << " please init IDetectRecogSink " << std::endl;
    exit(-1);
  }
}

vector<vector<float>> MotTracker::CalcFeatures(Mat frame_color, vector<Rect> rects) {
  vector<vector<float>> feats;
  if (sink_) {
    auto features = sink_->OnCalcFeatures(frame_color, rects);
    for (auto &feature : features) {
      int dim = feature.size() == 2064 ? 512 : 128;

      std::vector<float> decrypt(dim);
      if (arctern::encry2x(decrypt.data(), dim,
                           const_cast<uint8_t *>(feature.data()), feature.size()) ==
          arctern::ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
        std::cout << "codelength mismath" << std::endl;
      }
      feats.push_back(decrypt);
    }
  } else {
    std::cout << " please init IDetectRecogSink " << std::endl;
    exit(-1);
  }

  return feats;
}

void MotTracker::predict(Mat frame_gray, Mat frame_color, int frame_id) {
  for (size_t i = 0; i < tracked_stracks_.size(); i++) {
    tracked_stracks_[i]->predict();
    tracked_stracks_[i]->self_tracking(frame_gray, frame_color, frame_id);
  }
  for (size_t i = 0; i < lost_stracks_.size(); i++) {
    lost_stracks_[i]->predict();
  }
}

vector<std::shared_ptr<STracker>> MotTracker::detect(Mat frame_color, int frame_id) {
  vector<Rect> detections = Detect(frame_color, frame_id_);  // g_map_detect[frame_id];
  vector<vector<float>> feats = CalcFeatures(frame_color, detections);
  if (detections.size() != feats.size()) {
    std::cout << "detections size not equal feats size" << std::endl;
  }

  for (auto it = feats.begin(); it != feats.end(); it++) {
    for (auto vec = (*it).begin(); vec != (*it).end(); vec++) {
    }
  }
  vector<std::shared_ptr<STracker>> detection_strackers;
  for (size_t i = 0; i < detections.size(); i++) {
    // STracker stracker_temp(detections[i], 100, true, "SLK", 0.0, use_tracking_);
    KalmanFileterParams kalman_filter_params;
    std::shared_ptr<STracker> stracker_temp(new STracker(detections[i],
                                                    true,
                                                    param_.single_tracker_type,
                                                    param_.min_approved_overlap_ratio,
                                                    param_.use_tracking,
                                                    kalman_filter_params));
    stracker_temp->set_feature(feats[i]);
    detection_strackers.push_back(stracker_temp);
  }
  return detection_strackers;
}

void MotTracker::cal_reid_distance(const vector<std::shared_ptr<STracker>> &tracked_stracks,
                                   const vector<std::shared_ptr<STracker>> &detection_stracks,
                                   double *dists) {
  for (size_t i = 0; i < tracked_stracks.size(); i++) {
    for (size_t j = 0; j < detection_stracks.size(); j++) {
      vector<float> track_feature = tracked_stracks[i]->get_feature();
      vector<float> detection_feature = detection_stracks[j]->get_feature();
      double similarity = CalcSimilarity(&track_feature, &detection_feature);
      similarity = (similarity < 0.01) ? 0.01 : similarity;
      dists[i * detection_stracks.size() + j] = similarity;
      // int tracker_id = tracked_stracks[i]->get_track_id();
    }
  }
}

float cal_regular_euclidean_dist(Rect rect1, Rect rect2, vector<float> variences) {
  float r1x = rect1.x;
  float r1y = rect1.y;
  float r1w = rect1.width;
  float r1h = rect1.height;
  float r2x = rect2.x;
  float r2y = rect2.y;
  float r2w = rect2.width;
  float r2h = rect2.height;
  float dist_e2[4];
  dist_e2[0] = pow(r1x - r2x, 2);
  dist_e2[1] = pow(r1y - r2y, 2);
  dist_e2[2] = pow(r1w - r2w, 2);
  dist_e2[3] = pow(r1h - r2h, 2);
  float dist = 0;
  for (int i = 0; i < 4; i++) {
    dist += dist_e2[i] / variences[i];
  }
  dist = sqrt(dist);
  return dist;
}

// regular euclidean distance for matches where tracker is prediction and detection is real
void MotTracker::cal_pos_distance(const vector<shared_ptr<STracker>> &tracked_stracks,
                                  const vector<shared_ptr<STracker>> &detection_stracks,
                                  double *dists,
                                  float gating_thres,
                                  vector<float> variences) {
  for (size_t i = 0; i < tracked_stracks.size(); i++) {
    for (size_t j = 0; j < detection_stracks.size(); j++) {
      Rect track_rect = tracked_stracks[i]->get_rect();
      Rect detection_rect = detection_stracks[j]->get_rect();
      float dist_pos = cal_regular_euclidean_dist(track_rect, detection_rect, variences);
      if (dist_pos > gating_thres) {
        dists[i * detection_stracks.size() + j] = 0.01;
      }
    }
  }
}

// get output
vector<TrackInfo> MotTracker::get_output() {
  vector<TrackInfo> output_info;
  for (size_t i = 0; i < tracked_stracks_.size(); i++) {
    if ((param_.use_tracking == true) && (tracked_stracks_[i]->get_single_tracking_state() != SingleState::STracked)) {
      continue;
    }
    if ((param_.use_tracking == false) && (frame_id_ % param_.detect_period != 0)) {
      continue;
    }
    TrackInfo info_temp;
    info_temp.rect = tracked_stracks_[i]->get_rect();
    info_temp.rect_history = tracked_stracks_[i]->get_rect_history();
    info_temp.id = tracked_stracks_[i]->get_track_id();
    info_temp.from_det = tracked_stracks_[i]->get_from_det();
    info_temp.state = tracked_stracks_[i]->get_state();
    info_temp.frame_index = frame_id_;
    output_info.push_back(info_temp);
  }
  /*for(int i=0;i<lost_stracks_.size();i++){
      //if(lost_stracks_[i]->get_single_tracking_state()!=SingleState::STracked){
          //continue;
      //}
      TrackInfo info_temp;
      info_temp.rect = lost_stracks_[i]->get_rect();
      info_temp.id =  lost_stracks_[i]->get_track_id();
      info_temp.from_det =  lost_stracks_[i]->get_from_det();
      info_temp.state = lost_stracks_[i]->get_state();
      output_info.push_back(info_temp);
  }*/
  return output_info;
}

vector<TrackInfo> MotTracker::update(Mat frame_gray, Mat frame_color) {
  // *ptr_log<<"MotTracker::update:" <<endl;
  // *ptr_log<<"Param[Mat frame_gray]" << frame_gray <<endl;
  // *ptr_log<<"Param[Mat frame_color]" << frame_color <<endl;
  vector<shared_ptr<STracker>> activated_stracks;
  vector<shared_ptr<STracker>> refind_stracks;
  vector<shared_ptr<STracker>> lost_stracks;
  vector<shared_ptr<STracker>> removed_stracks;
  // "step 1: prediction", include kalman filter and single object tracking
  predict(frame_gray, frame_color, frame_id_);

  // "step 2: detection and set feature"
  vector<shared_ptr<STracker>> detection_stracks;
  // cout << "=====================================" << endl;
  // cout << "frame_id: " << frame_id_ << "track_params_.detect_period："
  // << param_.detect_period << "track_params_.detect_id: " << param_.detect_id << endl;

  if (frame_id_ % param_.detect_period == param_.detect_id) {
    detection_stracks = detect(frame_color, frame_id_);
    // cout << "MotTracker::update: all right here, continue..." << endl;
  } else {
    // cout << "MotTracker::update: unexpectedly returned" << endl;
    vector<TrackInfo> output_info = get_output();
    frame_id_ += 1;
    return output_info;
  }
  // "step 3: association for tracked"
  vector<shared_ptr<STracker>> unconfirmed;
  vector<shared_ptr<STracker>> tracked_stracks;
  vector<shared_ptr<STracker>> still_tracked_stracks;
  tracked_stracks = tracked_stracks_;
  /*for(int i=0;i<tracked_stracks_.size();i++){
      if(tracked_stracks_[i].check_activated()!=true){
          unconfirmed.push_back(tracked_stracks_[i]);
      }else{
          tracked_stracks.push_back(tracked_stracks_[i]);
      }
  }*/
  // matching for tracked_stracks;
  double *dists = new double[detection_stracks.size() * tracked_stracks.size()];
  cal_reid_distance(tracked_stracks, detection_stracks, dists);
  cal_pos_distance(tracked_stracks, detection_stracks, dists, param_.gating_thres, variences_);
  HungarianAssigning assigner;
  AssignResult assign_result =
      assigner.getAssignResult(dists, tracked_stracks.size(), detection_stracks.size(), param_.min_ap_dist);
  delete[] dists;
  for (size_t i = 0; i < assign_result.match_ids.size(); i++) {
    tracked_stracks[assign_result.match_ids[i].row_id]->update(detection_stracks[assign_result.match_ids[i].col_id],
                                                               frame_gray,
                                                               frame_color,
                                                               frame_id_);
  }
  vector<shared_ptr<STracker>> detection_stracks_copy = detection_stracks;
  vector<shared_ptr<STracker>> tracked_stracks_copy = tracked_stracks;
  for (size_t i = 0; i < assign_result.match_ids.size(); i++) {
    for (vector<shared_ptr<STracker>>::iterator iter = detection_stracks.begin(); iter != detection_stracks.end();
         iter++) {
      if (*iter == detection_stracks_copy[assign_result.match_ids[i].col_id]) {
        detection_stracks.erase(iter);
        break;
      }
    }
    still_tracked_stracks.push_back(tracked_stracks_copy[assign_result.match_ids[i].row_id]);
    for (vector<shared_ptr<STracker>>::iterator iter = tracked_stracks.begin(); iter != tracked_stracks.end(); iter++) {
      if (*iter == tracked_stracks_copy[assign_result.match_ids[i].row_id]) {
        tracked_stracks.erase(iter);
        break;
      }
    }
  }
  for (size_t i = 0; i < tracked_stracks.size(); i++) {
    tracked_stracks[i]->mark_lost();
    lost_stracks.push_back(tracked_stracks[i]);
  }
  // matching for missing targets
  dists = new double[detection_stracks.size() * lost_stracks_.size()];
  cal_reid_distance(lost_stracks_, detection_stracks, dists);
  cal_pos_distance(lost_stracks_, detection_stracks, dists, param_.gating_thres, variences_);
  assign_result = assigner.getAssignResult(dists, lost_stracks_.size(), detection_stracks.size(), param_.min_ap_dist);
  delete[] dists;
  for (size_t i = 0; i < assign_result.match_ids.size(); i++) {
    lost_stracks_[assign_result.match_ids[i].row_id]->re_activate(detection_stracks[assign_result.match_ids[i].col_id],
                                                                  frame_gray,
                                                                  frame_color,
                                                                  frame_id_);
    refind_stracks.push_back(lost_stracks_[assign_result.match_ids[i].row_id]);
  }
  detection_stracks_copy = detection_stracks;
  for (size_t i = 0; i < assign_result.match_ids.size(); i++) {
    for (vector<shared_ptr<STracker>>::iterator iter = detection_stracks.begin(); iter != detection_stracks.end();
         iter++) {
      if (*iter == detection_stracks_copy[assign_result.match_ids[i].col_id]) {
        detection_stracks.erase(iter);
        break;
      }
    }
  }
  // step 4:init new stracks
  for (size_t i = 0; i < detection_stracks.size(); i++) {
    detection_stracks[i]->activate(frame_gray, frame_color, frame_id_, track_id_);
    activated_stracks.push_back(detection_stracks[i]);
    track_id_++;
  }
  // step 5:update state
  for (size_t i = 0; i < lost_stracks_.size(); i++) {
    if (frame_id_ - lost_stracks_[i]->end_frame() > param_.max_time_lost) {
      lost_stracks_[i]->mark_removed();
      removed_stracks.push_back(lost_stracks_[i]);
    }
  }
  vector<shared_ptr<STracker>> tracked_stracks_temp;
  tracked_stracks_temp.insert(tracked_stracks_temp.end(), activated_stracks.begin(), activated_stracks.end());
  tracked_stracks_temp.insert(tracked_stracks_temp.end(), still_tracked_stracks.begin(), still_tracked_stracks.end());
  tracked_stracks_temp.insert(tracked_stracks_temp.end(), refind_stracks.begin(), refind_stracks.end());
  tracked_stracks_ = tracked_stracks_temp;
  vector<shared_ptr<STracker>> lost_stracks_temp;
  for (size_t i = 0; i < lost_stracks_.size(); i++) {
    if (lost_stracks_[i]->get_state() == TrackState::Lost) {
      lost_stracks_temp.push_back(lost_stracks_[i]);
    }
  }
  lost_stracks_temp.insert(lost_stracks_temp.end(), lost_stracks.begin(), lost_stracks.end());
  lost_stracks_ = lost_stracks_temp;
  vector<TrackInfo> output_info = get_output();
  frame_id_ += 1;
//    *ptr_log<<"MotTracker::update: OutPutInfo Start" <<endl;
//    //cout << "MotTracker::update: OutPutInfo Start" << endl;
//    *ptr_log<<"==================================================================" <<endl;
//    cout <<"==================================================================" <<endl;

  for (auto it = output_info.begin(); it != output_info.end(); it++) {
    // *ptr_log << "info_temp.rect：" << (*it).rect.x << " " << (*it).rect.y << " "
    // << (*it).rect.width << " " << (*it).rect.height << " " << endl;
    // *ptr_log << "info_temp.id：" << (*it).id << endl;
    // *ptr_log << "info_temp.frame_index: " << (*it).frame_index << endl;
    // *ptr_log << "info_temp.from_det：" << (*it).from_det << endl;
    // *ptr_log << "info_temp.state：" << (*it).state << endl;


//        cout << "info_temp.rect：" << (*it).rect.x << " " << (*it).rect.y << " "
//        << (*it).rect.width << " " << (*it).rect.height << " " << endl;
//        cout << "info_temp.id：" << (*it).id << endl;
//        cout << "info_temp.frame_index: " << (*it).frame_index << endl;
//        cout << "info_temp.from_det：" << (*it).from_det << endl;
//        cout << "info_temp.state：" << (*it).state << endl;
  }
//    cout << "output_info.size(): " << output_info.size() << endl;
//    *ptr_log<<"==================================================================" <<endl;
  return output_info;
}

