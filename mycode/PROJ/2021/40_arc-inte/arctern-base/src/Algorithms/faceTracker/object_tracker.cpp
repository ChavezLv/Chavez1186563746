/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceTracker/object_tracker.h"

using arctern::ObjectTracker;
using arctern::TrackResult;

ObjectTracker::ObjectTracker(const ObjectTrackerParameter &object_tracker_params)
    : track_params_(object_tracker_params),
      mot_tracker_(std::make_shared<MotTracker>(object_tracker_params)) {}

TrackResult ObjectTracker::track(const cv::Mat &image) {
  frame_height_ = image.rows;
  cv::Mat image_gray;
  cvtColor(image, image_gray, CV_BGR2GRAY);
  std::vector<TrackInfo> track_info_list =
      mot_tracker_->update(image_gray, image);
  std::vector<TrackerInfo> tracking_result_list;
  for (size_t i = 0; i < track_info_list.size(); ++i) {
    TrackerInfo tracking_result;
    tracking_result.rect = track_info_list[i].rect;
    tracking_result.rect_history = track_info_list[i].rect_history;
    tracking_result.id = track_info_list[i].id;
    tracking_result.from_det = track_info_list[i].from_det;
    tracking_result.frame_index = track_info_list[i].frame_index;

    tracking_result.state = TrackerState(static_cast<int>(track_info_list[i].state));

    tracking_result_list.push_back(tracking_result);
  }

  auto cnt = counting(tracking_result_list);
  cnt.tracking_infos = std::move(tracking_result_list);
  ++frame_index_now_;

  return cnt;
}

float ObjectTracker::tracy_dist(std::vector<cv::Rect> rect_history) {
  float dist = 0.0;
  for (size_t i = 1; i < rect_history.size(); i++) {
    float square_dist = std::pow(rect_history[i].x - rect_history[i - 1].x, 2) +
        pow(rect_history[i].y - rect_history[i - 1].y, 2);
    float dist_temp = std::sqrt(square_dist);
    dist += dist_temp;
  }

  return dist;
}

TrackResult ObjectTracker::counting(
    const std::vector<TrackerInfo> &track_info_list) {
  TrackResult tr;
  tr.tracked_count = 0;
  // update track_info
  for (size_t i = 0; i < track_info_list.size(); i++) {
    bool exist_flag = false;
    for (size_t j = 0; j < tracking_info_.size(); j++) {
      if (track_info_list[i].id == tracking_info_[j].id) {
        tracking_info_[j] = track_info_list[i];
        std::cout << " tracking_info status = " << track_info_list[i].state << std::endl;
        exist_flag = true;
        break;
      }
    }
    if (exist_flag == false) {
      tracking_info_.push_back(track_info_list[i]);
    }
  }

  // looking for tracked results
  for (auto iter = tracking_info_.begin(); iter != tracking_info_.end();) {
    int last_update_frame_index = (*iter).frame_index;
    if ((frame_index_now_ - last_update_frame_index) >=
        track_params_.max_time_lost) {
      tr.tracked_infos.push_back(*iter);
      float dist = tracy_dist(iter->rect_history);
      if ((dist > 0.5 * frame_height_) && (frame_height_ > 1.0)) {
        tr.tracked_count += 1;
      }
      iter = tracking_info_.erase(iter);
    } else {
      iter++;
    }
  }

  return tr;
}

void ObjectTracker::initDetectRecogSink(arctern::IDetectRecogSink *sink) {
  mot_tracker_->sink_ = sink;
}
