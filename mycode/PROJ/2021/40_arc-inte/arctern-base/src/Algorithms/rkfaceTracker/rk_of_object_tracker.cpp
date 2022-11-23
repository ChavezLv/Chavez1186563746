#include "rk_of_object_tracker.h"

extern "C" {
#include "if_algo_api.h"
#include "arm/arm_api.h"
}

#include <algorithm>
#include <thread>
#include <chrono>

using namespace arctern;

struct RKOpticalFlowObjectTracker::State {
  const RKOpticalFlowObjectTracker::ExtSetting *setting_;
  s16 face_cycle_id_;
  int current_frame_index_;
  t_track_face_deil deil_[2];
  int deil_ptr_;

  void update(cv::Mat &curr_img, const cv::Rect *rect, std::size_t n) {
    t_track_face_deil &d = deil_[deil_ptr_];
    t_track_face_deil &up = deil_[1 - deil_ptr_];
    fill_deil(rect, n, up);
    for (s16 i = 0; i < up.face_num; ++i) {
      s16 slot = find_nearest(&up.face_roi[i][0], (s16 *) d.face_roi, d.face_num);
      if (slot >= 0) { // amend
        up.face_id[i] = d.face_id[slot];
        std::cout << "amend " << up.face_id[i] << std::endl;
        if (slot != d.face_num - 1) {
          d.face_id[slot] = d.face_id[d.face_num - 1];
          d.face_roi[slot][0] = d.face_roi[d.face_num - 1][0];
          d.face_roi[slot][1] = d.face_roi[d.face_num - 1][1];
          d.face_roi[slot][2] = d.face_roi[d.face_num - 1][2];
          d.face_roi[slot][3] = d.face_roi[d.face_num - 1][3];
        }
        d.face_num--;
      } else { // added new
        up.face_id[i] = ++face_cycle_id_;
        std::cout << "added " << up.face_id[i] << std::endl;
      }
    }
    deil_ptr_ = 1 - deil_ptr_;
    current_frame_index_++;
  }

  void track(cv::Mat &prev_image, cv::Mat &curr_img) {
    if (prev_image.empty() || curr_img.empty())
      return;
    t_track_face_deil &in = deil_[deil_ptr_];
    t_track_face_deil &out = deil_[1 - deil_ptr_];
    do_track_algo(prev_image, curr_img, in, out);
    deil_ptr_ = 1 - deil_ptr_;
    current_frame_index_++;
  }

  State(const RKOpticalFlowObjectTracker::ExtSetting *es) : setting_(es) {
    deil_[0].face_num = 0;
    deil_[1].face_num = 0;
    deil_ptr_ = 0;
    current_frame_index_ = 0;
    face_cycle_id_ = 0;
  }

  State(const State &s) : setting_(s.setting_) {
    face_cycle_id_ = s.face_cycle_id_;
    current_frame_index_ = s.current_frame_index_;
    deil_ptr_ = s.deil_ptr_;
    memcpy(&deil_[deil_ptr_].face_id[0], &s.deil_[deil_ptr_].face_id[0], sizeof(s16) * s.deil_[deil_ptr_].face_num);
    memcpy(&deil_[deil_ptr_].face_roi[0][0],
           &s.deil_[deil_ptr_].face_roi[0][0],
           sizeof(s16) * 4 * s.deil_[deil_ptr_].face_num);
    deil_[deil_ptr_].face_num = s.deil_[deil_ptr_].face_num;
  }

  TrackResult get_result() {
    // translate t_track_face_deil to TrackResult
    TrackResult result;
    t_track_face_deil &curr = deil_[deil_ptr_];
    result.tracked_count = curr.face_num;
    result.tracking_infos.reserve(curr.face_num);
    for (int i = 0; i < curr.face_num; ++i) {
      TrackerInfo ti;
      ti.from_det = false;
      ti.id = curr.face_id[i];
      ti.frame_index = current_frame_index_;
      ti.rect = cv::Rect(cv::Point((int) curr.face_roi[i][0], (int) curr.face_roi[i][1]),
                         cv::Point((int) curr.face_roi[i][2], (int) curr.face_roi[i][3]));
      ti.rect.width++;
      ti.rect.height++;
      ti.state = arctern::TrackerState::TRACKED;
      result.tracking_infos.push_back(ti);
    }
    return result;
  }
 private:
  void do_track_algo(cv::Mat &prev_img, cv::Mat &curr_img, t_track_face_deil &in, t_track_face_deil &out) {
    // step1. call SVP_ARM_FaceTrack  to collect latest face ROIs
    // step2. justify face box to be valid
    // step3. erase invalid face box returned from SVP_ARM_FaceTrack
    if (in.face_num > 0) {
      s16 h = curr_img.rows;
      s16 w = curr_img.cols;
      SVP_ARM_FaceTrack((u64) prev_img.data, (u64) curr_img.data,
                        (void *) in.face_roi, (void *) in.face_id, (void *) out.face_roi, (void *) out.face_id,
                        in.face_num, &out.face_num, h, w, setting_->track_params_.min_obj_size);
      for (s16 i = 0; i < out.face_num; ++i) {
        correct_box(&out.face_roi[i][0], w, h);
      }
      s16 tail = out.face_num - 1;
      for (s16 i = tail; i >= 0; --i) {
        if (!test_valid(&out.face_roi[i][0])) {
          // erase by swapping with tail element
          if (i != tail) {
            out.face_id[i] = out.face_id[tail];
            out.face_roi[i][0] = out.face_roi[tail][0];
            out.face_roi[i][1] = out.face_roi[tail][1];
            out.face_roi[i][2] = out.face_roi[tail][2];
            out.face_roi[i][3] = out.face_roi[tail][3];
          }
          tail--;
        }
      }
      out.face_num = tail + 1;
    } else {
      out.face_num = 0;
    }
  }
  void fill_deil(const cv::Rect *rect, std::size_t n, t_track_face_deil &d) {
    s16 k = 0;
    int min_obj_size = setting_->track_params_.min_obj_size;
    for (std::size_t i = 0; i < n; ++i) {
      d.face_id[k] = 0;
      d.face_roi[k][0] = (s16) rect[i].x;
      d.face_roi[k][1] = (s16) rect[i].y;
      d.face_roi[k][2] = (s16) (rect[i].x + rect[i].width - 1);
      d.face_roi[k][3] = (s16) (rect[i].y + rect[i].height - 1);
      if (test_valid(&d.face_roi[k][0])) {
        k++;
      }
    }
    d.face_num = k;
  }
  static void correct_box(s16 *box, s16 w, s16 h) {
    s16 x0 = box[0];
    s16 x1 = box[2];
    s16 y0 = box[1];
    s16 y1 = box[3];
    if (x0 < 0)
      x0 = 0;
    else if (x0 >= w)
      x0 = w - 1;
    if (x1 < 0)
      x1 = 0;
    else if (x1 >= w)
      x1 = w - 1;
    if (y0 < 0)
      y0 = 0;
    else if (y0 >= h)
      y0 = h - 1;
    if (y1 < 0)
      y1 = 0;
    else if (y1 >= h)
      y1 = h - 1;
    box[0] = x0;
    box[1] = y0;
    box[2] = x1;
    box[3] = y1;
  }
  static bool test_border_ratio_valid(int w, int h, int ratio) {
    return 1024 * std::abs(w - h) < ratio * std::max(w, h);
  }
  bool test_valid(s16 *box) {
    s16 x0 = box[0];
    s16 x1 = box[2];
    s16 y0 = box[1];
    s16 y1 = box[3];
    s16 w = x1 - x0 + 1;
    s16 h = y1 - y0 + 1;
    if (w < setting_->track_params_.min_obj_size)
      return false;
    if (h < setting_->track_params_.min_obj_size)
      return false;
    return test_border_ratio_valid(w, h, setting_->border_rate_thres);
  }
  s16 find_nearest(s16 *target, s16 *box, s16 n) {
    s32 slot = -1;
    s32 min_dist = 0x7fffffff;
    for (s16 i = 0; i < n; ++i) {
      s32 dist = face_distance(target, box + 4 * i);
      if (min_dist > dist) {
        min_dist = dist;
        slot = i;
      }
    }
    if (slot >= 0) {
      s32 A = face_area(target);
      s32 B = face_area(box + 4 * slot);
      s32 C = min_dist;
      if ((A + B - C) * 1024 >= (A + B + C) * setting_->overlap_thres_amend) {
        return slot;
      }
    }
    return -1;
  }
  static s32 face_distance(s16 *a, s16 *b) {
    s32 dx = a[0] + a[2] - b[0] - b[2];
    s32 dy = a[1] + a[3] - b[1] - b[3];
    return dx * dx + dy * dy;
  }
  static s32 face_area(s16 *a) {
    return ((s32) (a[2] - a[0] + 1)) * ((s32) (a[3] - a[1] + 1));
  }
};

//
// RKOpticalFlowObjectTracker
//

RKOpticalFlowObjectTracker::RKOpticalFlowObjectTracker(RKOpticalFlowObjectTracker &tracker) {
  setting_ = tracker.setting_;
  state_.reset(new RKOpticalFlowObjectTracker::State(*tracker.state_.get()));
  prev_frame_ = tracker.prev_frame_;
}

RKOpticalFlowObjectTracker::RKOpticalFlowObjectTracker(const ObjectTrackerParameter &object_tracker_params) {
  setting_.reset(new ExtSetting());
  setting_->overlap_thres_amend = (int) (0.63 * 1024);
  setting_->track_params_ = object_tracker_params;
  if (setting_->track_params_.min_obj_size <= 0) {
    setting_->track_params_.min_obj_size = 20;
  }
  setting_->border_rate_thres = (int) (0.4 * 1024);
  state_.reset(new RKOpticalFlowObjectTracker::State(setting_.get()));
}

RKOpticalFlowObjectTracker::~RKOpticalFlowObjectTracker() {
}

TrackResult RKOpticalFlowObjectTracker::track(cv::Mat &image) {
  if (image.type() != CV_8UC1) {
    std::cerr << "error: you must be 8UC1" << std::endl;
    throw std::runtime_error("RKOpticalFlowObjectTracker::track image format invalid!");
  }
  state_->track(prev_frame_, image);
  prev_frame_ = image.clone();
  return state_->get_result();
}

TrackResult RKOpticalFlowObjectTracker::update(cv::Mat &image, const std::vector<cv::Rect> &detection_results) {
  if (image.type() != CV_8UC1) {
    std::cerr << "error: you must be 8UC1" << std::endl;
    throw std::runtime_error("RKOpticalFlowObjectTracker::update image format invalid!");
  }
  state_->update(image, detection_results.data(), std::min(detection_results.size(), (std::size_t) TRACK_FACE_NUM_MAX));
  prev_frame_ = image.clone();
  return state_->get_result();
}
