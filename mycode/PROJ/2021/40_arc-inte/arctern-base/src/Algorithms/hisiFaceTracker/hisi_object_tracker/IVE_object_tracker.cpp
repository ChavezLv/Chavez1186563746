//#ifdef __arm__

#if defined(AARCH64_HIMIX100) || defined(AARCH64_HIMIX200)

#include <iostream>
extern "C" {
#include <hi_sdk/hi_comm_sys.h>
#include <hi_sdk/mpi_sys.h>
#include <hi_sdk/hi_type.h>
}
#include "IVE_object_tracker.h"
#include "utils.h"

#include <sys/time.h>
#include <set>


using namespace arctern;

//private
void rects_to_track_face_deil_(const std::vector<cv::Rect> &rects, t_track_face_deil &track_face_deil) {
  track_face_deil.face_num = rects.size();
  for (int idx = 0; idx < track_face_deil.face_num; ++idx) {
    const cv::Rect &box = rects[idx];
    track_face_deil.face_roi[idx][0] = box.x;
    track_face_deil.face_roi[idx][1] = box.y;
    track_face_deil.face_roi[idx][2] = box.br().x;
    track_face_deil.face_roi[idx][3] = box.br().y;
    track_face_deil.face_id[idx] = 0; // no need for face id
  }
}
//
//void boxes_to_track_face_deil_(const Vbbox &boxes, t_track_face_deil &track_face_deil) {
//  track_face_deil.face_num = boxes.size();
//  for (int idx = 0; idx < track_face_deil.face_num; ++idx) {
//    const Bbox &box = boxes[idx];
//    track_face_deil.face_roi[idx][0] = box.x1;
//    track_face_deil.face_roi[idx][1] = box.y1;
//    track_face_deil.face_roi[idx][2] = box.x2;
//    track_face_deil.face_roi[idx][3] = box.y2;
//  }
//}

void tracker_infos_to_face_deil_(const std::vector<TrackerInfo> tracker_infos, t_track_face_deil &track_face_deil) {
  track_face_deil.face_num = tracker_infos.size();
  for (int idx = 0; idx < track_face_deil.face_num; ++idx) {
    const cv::Rect &rect = tracker_infos[idx].rect;
    int face_id = tracker_infos[idx].id;
    track_face_deil.face_roi[idx][0] = rect.x;
    track_face_deil.face_roi[idx][1] = rect.y;
    track_face_deil.face_roi[idx][2] = rect.br().x;
    track_face_deil.face_roi[idx][3] = rect.br().y;
    track_face_deil.face_id[idx] = face_id;
  }
}

void face_deil_to_tracker_infos_(const t_track_face_deil &track_face_deil, std::vector<TrackerInfo> &tracker_infos, const cv::Size &frame_size, e_sync_face_status *status) {
  tracker_infos.clear();
  for (int idx = 0; idx < track_face_deil.face_num; ++idx) {
    //if (status && status[idx] != 0) { continue; }

    const HI_S16 *face_roi = track_face_deil.face_roi[idx];
    cv::Rect face_rect = cv::Rect(cv::Point(face_roi[0], face_roi[1]), cv::Point(face_roi[2], face_roi[3]));

    // check rect
    if (face_rect.x < 0) { face_rect.x = 0; }
    if (face_rect.y < 0) { face_rect.y = 0; }
    if (face_rect.x + face_rect.width >= frame_size.width) { 
      face_rect.width = frame_size.width - face_rect.x;
    }
    if (face_rect.y + face_rect.height >= frame_size.height) { 
      face_rect.height = frame_size.height - face_rect.y;
    }
    if (face_rect.width <= 0 || face_rect.height <= 0) { continue; }

    TrackerInfo tracker_info;
    tracker_info.id = track_face_deil.face_id[idx];
    tracker_info.rect = face_rect;

    tracker_infos.push_back(tracker_info);
  }
}

void IVEObjectTracker::check_and_update_cache_(const t_track_face_deil &deil, const cv::Size &frame_size) {
  //pay special attension that all objects in deil are currently being tracked, so our update process will be two steps:
  //step 1: update all tracking objects
  //step 2: find out disappeared objects, mark them as untracked
  
  //step 1: for each object in deil, check new rect, update cache
  std::set<int> tracking_face_ids;
  for (int idx = 0; idx < deil.face_num; ++idx) {
    int face_id = deil.face_id[idx];
    const HI_S16 *face_roi = deil.face_roi[idx];

    cv::Rect face_rect = cv::Rect(cv::Point(face_roi[0], face_roi[1]), cv::Point(face_roi[2], face_roi[3]));

    // check rect
    if (face_rect.x < 0) { face_rect.x = 0; }
    if (face_rect.y < 0) { face_rect.y = 0; }
    if (face_rect.x + face_rect.width >= frame_size.width) { 
      face_rect.width = frame_size.width - face_rect.x;
    }
    if (face_rect.y + face_rect.height >= frame_size.height) { 
      face_rect.height = frame_size.height - face_rect.y;
    }
    if (face_rect.width <= 0 || face_rect.height <= 0) { continue; }

    TrackerInfo &tracker_info = tracking_map_[face_id];
    tracker_info.rect = face_rect;
    tracker_info.rect_history.push_back(face_rect);
    tracker_info.frame_index = current_frame_index_;
    tracker_info.id = face_id;
    tracker_info.state = arctern::TrackerState::TRACKED;

    tracking_face_ids.insert(face_id);
  }

  // step 2: for other objects in tracking_map_, marked them as untracked if necessary
  for (auto itr = tracking_map_.begin(); itr != tracking_map_.end();) {
    std::pair<int, TrackerInfo> tracker_info_pair = *itr;

    //for all pairs in tracking_map_, if their ids also appears in tracking_face_ids (the set we previously created), we say it's still being tracked, ignore them
    if (tracking_face_ids.find(tracker_info_pair.first) != tracking_face_ids.end()) { ++itr; continue; }

    //for those ids didn't appear in the current frame, if they disappears for a long time, mark them as untracked
    const TrackerInfo &tracker_info = tracker_info_pair.second;
    int last_update_frame_index = tracker_info.frame_index;
    if (current_frame_index_ - last_update_frame_index >= track_params_.max_time_lost) {
      tracker_info_pair.second.state = arctern::TrackerState::LOST;
      tracked_map_[tracker_info_pair.first] = tracker_info_pair.second;
      tracking_map_.erase(itr++);
    } else {
      ++itr;
    }
  }
}

//public
IVEObjectTracker::IVEObjectTracker(const ObjectTrackerParameter &object_tracker_params, int channel_index)
                                  : track_params_(object_tracker_params), channel_index_(channel_index) {
#ifndef AARCH64_HIMIX200_3516DV300
  if (track_params_.single_tracker_type == "KCF") {
    std::cerr << "[IVEObjectTracker]: IVE KCF not supported on 3559, change back to SLK." << std::endl;
    track_params_.single_tracker_type = "SLK";
  }
#endif // if def AARCH64_HIMIX100
  curr_faces_track_.face_num = 0;
  prev_faces_track_.face_num = 0;
  assert(track_params_.ive_input_img_width % 8 == 0 && track_params_.ive_input_img_height % 8 == 0);
  if (track_params_.min_obj_size <= 0) { track_params_.min_obj_size = 32; }

  if (track_params_.single_tracker_type == "SLK") {
    ive_lk_tracker_ = (IVE_LK_TRACK *) malloc(sizeof(IVE_LK_TRACK));
    if_IVE_LK_init(ive_lk_tracker_, track_params_);
  } else if (track_params_.single_tracker_type == "KCF") {
    ive_kcf_tracker_ = (IVE_KCF_TRACK *) malloc(sizeof(IVE_KCF_TRACK));
    if_IVE_KCF_init(ive_kcf_tracker_, track_params_);
  } else {
    std::cerr << "IVEObjectTracker: unsupported single_tracker_type, please choose SLK or KCF." << std::endl;
    assert(false);
  }

  if_algo_init(track_params_.min_obj_size);
  if (track_params_.device_type == DT_DSP) {
    if_algo_dsp_on(/*track_params_.dsp_bin_root.c_str()*/);
  }
}

//IVEObjectTracker::IVEObjectTracker(const std::shared_ptr<Detector> &detector,
//                                  const ObjectTrackerParameter &object_tracker_params,
//                                   int channel_index)
//               : track_params_(object_tracker_params), detector_(detector), channel_index_(channel_index) {
//  assert(track_params_.ive_input_img_width % 8 == 0 && track_params_.ive_input_img_height % 8 == 0);
//  if (track_params_.min_obj_size <= 0) { track_params_.min_obj_size = 32; }
//
//  if (track_params_.single_tracker_type == "SLK") {
//    ive_lk_tracker_ = (IVE_LK_TRACK *) malloc(sizeof(IVE_LK_TRACK));
//    if_IVE_LK_init(ive_lk_tracker_, track_params_);
//  } else if (track_params_.single_tracker_type == "KCF") {
//    ive_kcf_tracker_ = (IVE_KCF_TRACK *) malloc(sizeof(IVE_KCF_TRACK));
//    if_IVE_KCF_init(ive_kcf_tracker_, track_params_);
//  }
//
//  if_algo_init(track_params_.min_obj_size);
//  if (track_params_.device_type == DT_DSP) {
//    if_algo_dsp_on(/*track_params_.dsp_bin_root.c_str()*/);
//  }
//}

IVEObjectTracker::~IVEObjectTracker() {
  if (track_params_.single_tracker_type == "SLK") {
    if_IVE_LK_uninit(ive_lk_tracker_);
  } else if (track_params_.single_tracker_type == "KCF") {
    if_IVE_KCF_uninit(ive_kcf_tracker_);
  }
  if (track_params_.device_type == DT_DSP) {
    if_algo_dsp_off();
  }
}

void IVEObjectTracker::trans_data_to_LK(const cv::Mat &gray_image, const cv::Size &orig_size) {
  //bad function
  //this method is used for translating the data from src to ive.
  //if the source data is stored in ive already, we can trans the data without this method
  //just make sure s_stStLk->stTmp.au32Stride[0] has data before tracking
  HI_U8* dst_pointer = (HI_U8 *)(HI_UL)ive_lk_tracker_->stTmp.au64VirAddr[0];
  HI_U8* src_pointer = (HI_U8*)gray_image.data;

  HI_U16 height = gray_image.rows;
  ive_lk_tracker_->stTmp.u32Height = height;
  HI_U16 width = gray_image.cols;
  ive_lk_tracker_->stTmp.u32Width = width;

  ive_lk_tracker_->img_change_scale = ((float) track_params_.ive_input_img_width) / ((float) orig_size.width);
  ive_lk_tracker_->img_change_scale_h = ((float) track_params_.ive_input_img_height) / ((float) orig_size.height);

  for (HI_U32 y = 0; y < height; y++) {
    memcpy((HI_VOID*)dst_pointer, (HI_VOID*)src_pointer, sizeof(HI_U8) * width);
    dst_pointer += ive_lk_tracker_->stTmp.au32Stride[0];
    src_pointer += gray_image.step[0];
  }
}

void IVEObjectTracker::trans_data_to_KCF(const cv::Mat &yuv_image, const cv::Size &orig_size) {
  //bad function
  //this method is used for translating the data from src to ive.
  //if the source data is stored in ive already, we can trans the data without this method
  if (ive_kcf_tracker_->stSrcImg.u32Width != yuv_image.cols ||
     ive_kcf_tracker_->stSrcImg.u32Height * 3 / 2 != yuv_image.rows) {
    std::cerr <<"[ive_object_tracker::trans_data_to_kcf]: wrong image size"<< std::endl;
    return;
  }

  memcpy((HI_U8*)(HI_UL)ive_kcf_tracker_->stSrcImg.au64VirAddr[0], (HI_VOID*)yuv_image.data, yuv_image.step[0] * yuv_image.rows);
}

void IVEObjectTracker::track(const cv::Mat &image) {
  TIME_DEBUG_START("[IVEObjectTracker::track]: start");
  cv::Mat resized_gray_image = convertGrayMat(image);

  //todo no det
#if 0
  TIME_DEBUG_LOG("[IVEObjectTracker::track]: preprocess done");
  if (detector_ && current_frame_index_ % track_params_.detect_period == track_params_.detect_id) {
    //detect
    auto detection_result = detector_->detect(image, nullptr);

    boxes_to_track_face_deil_(detection_result, prev_faces_track_);
    if (track_params_.single_tracker_type == "SLK") {
      trans_data_to_LK(resized_gray_image, cv::Size(gray_image.cols, gray_image.rows));
      if_IVE_LK_update(ive_lk_tracker_, prev_faces_track_);
    } else if(track_params_.single_tracker_type == "KCF") {
      t_track_face_deil new_detection_boxes;
      new_detection_boxes.face_num = 0;
      if_IVE_KCF_update(ive_kcf_tracker_, prev_faces_track_, new_detection_boxes);
    }

    s32 track_lock_status = ALGO_FAILURE;
    do {
      track_lock_status = track_res_lock(track_params_.channel_idx);
    } while (track_lock_status == ALGO_FAILURE);
    e_sync_face_status sync_status[TRACK_FACE_NUM_MAX] = {E_FACE_DEFAULT};
    
    track_res_sync_detect(&prev_faces_track_, sync_status, &curr_faces_track_, track_params_.channel_idx, current_frame_index_);
    track_res_unlock(track_params_.channel_idx);

    check_and_update_cache_(prev_faces_track_, cv::Size(image.cols, image.rows));
  } else {
#endif
    //track
    s16 *face_roi_tmp = (s16 *) malloc(TRACK_FACE_NUM_MAX * 4 * sizeof(s16));
    s32 *face_id_tmp = (s32 *) malloc(TRACK_FACE_NUM_MAX * sizeof(s32));
    s16 face_num;
    s16 track_lock_status = ALGO_FAILURE;
    do {
      track_lock_status = track_res_lock(track_params_.channel_idx);
    } while (track_lock_status == ALGO_FAILURE);
    track_res_get(track_params_.channel_idx, face_roi_tmp, face_id_tmp, &face_num);

    if(track_params_.single_tracker_type == "SLK") {
      trans_data_to_LK(resized_gray_image, cv::Size(image.cols, image.rows));
      if_IVE_LK_track(ive_lk_tracker_, prev_faces_track_);
    } else if(track_params_.single_tracker_type == "KCF") {
      if_IVE_KCF_track(ive_kcf_tracker_, prev_faces_track_);
    }
    check_and_update_cache_(prev_faces_track_, cv::Size(image.cols, image.rows));

    track_res_update_custom_timeout(&prev_faces_track_, track_params_.channel_idx, current_frame_index_, track_params_.max_time_lost);
    track_res_unlock(track_params_.channel_idx);
    free(face_roi_tmp);
    free(face_id_tmp);

    //todo no detect
#if 0
  }
#endif
  TIME_DEBUG_LOG("[IVEObjectTracker::track]: track done");
  ++current_frame_index_;
  TIME_DEBUG_END("[IVEObjectTracker::track]: postprocess done");
}

std::vector<TrackerInfo> IVEObjectTracker::non_detection_track(const cv::Mat &image) {
//  struct timeval stt, ent;
  
  TIME_DEBUG_START("[ive_object_tracker::track-2]: started");
  cv::Mat  resized_gray_image;
  cv::Mat resized_yuv_image;
  if (track_params_.single_tracker_type == "SLK") {
    resized_gray_image = convertGrayMat(image);
  } else if(track_params_.single_tracker_type == "KCF") {
    resized_yuv_image = convertYUVMat(image);
  }
  TIME_DEBUG_LOG("[ive_object_tracker::track-2]: preprocess ended");

  std::vector<TrackerInfo> tracking_infos_result;
  if (current_frame_index_ > 0) {
    s16 *face_roi_tmp = (s16 *) malloc(TRACK_FACE_NUM_MAX * 4 * sizeof(s16));
    s32 *face_id_tmp = (s32 *) malloc(TRACK_FACE_NUM_MAX * sizeof(s32));
    s16 face_num;
    s16 track_lock_status = ALGO_FAILURE;
    do {
      track_lock_status = track_res_lock(track_params_.channel_idx);
    } while (track_lock_status == ALGO_FAILURE);
    track_res_get(track_params_.channel_idx, face_roi_tmp, face_id_tmp, &face_num);

    //tracker_infos_to_face_deil_(tracking_infos, prev_faces_track_);
    if (track_params_.single_tracker_type == "SLK") {
      trans_data_to_LK(resized_gray_image, cv::Size(image.cols, image.rows));
      if_IVE_LK_track(ive_lk_tracker_, prev_faces_track_);
    } else if(track_params_.single_tracker_type == "KCF") {
      trans_data_to_KCF(resized_yuv_image, cv::Size(image.cols, image.rows));
      if_IVE_KCF_track(ive_kcf_tracker_, prev_faces_track_);
    }

    face_deil_to_tracker_infos_(prev_faces_track_, tracking_infos_result, cv::Size(image.cols, image.rows), NULL);

    track_res_update_custom_timeout(&prev_faces_track_, track_params_.channel_idx, current_frame_index_, track_params_.max_time_lost);
    track_res_unlock(track_params_.channel_idx);
    free(face_roi_tmp);
    free(face_id_tmp);
  } // if has no prev_frame_, no tracking performed, simply records this frame
  TIME_DEBUG_LOG("[ive_object_tracker::track-2]: track ended");

  ++current_frame_index_;
  TIME_DEBUG_END("[ive_object_tracker::track-2]: postprocess ended");

  return tracking_infos_result;
}

std::vector<TrackerInfo> IVEObjectTracker::non_detection_update(const cv::Mat &image, const std::vector<cv::Rect> &rects) {
  TIME_DEBUG_START("[ive_object_tracker::update]: started");
  cv::Mat resized_gray_image;
  cv::Mat resized_yuv_image;
  if (track_params_.single_tracker_type == "SLK") {
     resized_gray_image = convertGrayMat(image);
  } else if (track_params_.single_tracker_type == "KCF") {
    resized_yuv_image = convertYUVMat(image);
  }
  TIME_DEBUG_LOG("[ive_object_tracker::update]: preprocess ended");

  track_res_update_custom_timeout(&prev_faces_track_, track_params_.channel_idx, current_frame_index_, track_params_.max_time_lost);
  rects_to_track_face_deil_(rects, prev_faces_track_);

  // lib_if_algo for target management: started
  s32 track_lock_status = ALGO_FAILURE;
  do {
    track_lock_status = track_res_lock(track_params_.channel_idx);
  } while (track_lock_status == ALGO_FAILURE);
  e_sync_face_status sync_status[TRACK_FACE_NUM_MAX] = {E_FACE_DEFAULT};
  track_res_sync_detect(&prev_faces_track_, sync_status, &curr_faces_track_, track_params_.channel_idx, current_frame_index_);
  track_res_unlock(track_params_.channel_idx);
  TIME_DEBUG_LOG("[ive_object_tracker::update]: update ended");
  // lib_if_algo for target management: ended, results stored in curr_faces_track_ and sync_status

  // curr_faces_track_ copy back to prev_faces_track_, we need things stored in prev_faces_track_ in 
  for (int idx = 0; idx < curr_faces_track_.face_num; ++idx) {
    prev_faces_track_.face_roi[idx][0] = curr_faces_track_.face_roi[idx][0];
    prev_faces_track_.face_roi[idx][1] = curr_faces_track_.face_roi[idx][1];
    prev_faces_track_.face_roi[idx][2] = curr_faces_track_.face_roi[idx][2];
    prev_faces_track_.face_roi[idx][3] = curr_faces_track_.face_roi[idx][3];
    prev_faces_track_.face_id[idx] = curr_faces_track_.face_id[idx];
  }
  prev_faces_track_.face_num = curr_faces_track_.face_num;

  if (track_params_.single_tracker_type == "SLK") {
    trans_data_to_LK(resized_gray_image, cv::Size(image.cols, image.rows));
    if_IVE_LK_update(ive_lk_tracker_, prev_faces_track_);
  } else if (track_params_.single_tracker_type == "KCF") {
    // here prev_faces_track_ and curr_faces_track_ have the same contents
    // we treat the whole curr_faces_track_ as new detected boxes
    trans_data_to_KCF(resized_yuv_image, cv::Size(image.cols, image.rows));
    if_IVE_KCF_update(ive_kcf_tracker_, prev_faces_track_, curr_faces_track_);
  }

  // curr_faces_track_ -> tracking_infos_result
  std::vector<TrackerInfo> tracking_infos_result;
  face_deil_to_tracker_infos_(curr_faces_track_, tracking_infos_result, cv::Size(image.cols, image.rows), sync_status);

  ++current_frame_index_;
  TIME_DEBUG_END("[ive_object_tracker::update]: write back and postprocess ended");

  return tracking_infos_result;
}

// pack and get results
const std::map<int, TrackerInfo> &IVEObjectTracker::get_tracking_map() {
  return tracking_map_;
}

const std::map<int, TrackerInfo> &IVEObjectTracker::get_tracked_map() {
  return tracked_map_;
}

TrackResult IVEObjectTracker::get_result() {
  TrackResult tr;
  tr.tracked_count = tracking_map_.size();
  for (auto itr = tracking_map_.begin(); itr != tracking_map_.end(); ++itr) {
    tr.tracking_infos.push_back(itr->second);
  }
  for (auto itr = tracked_map_.begin(); itr != tracked_map_.end(); ++itr) {
    tr.tracked_infos.push_back(itr->second);
  }
  return tr;
}

cv::Mat IVEObjectTracker::convertGrayMat(const cv::Mat & img) {
    cv::Mat gray_image, resized_gray_image;
   if (img.channels() == 3) {
      cv::cvtColor(img, gray_image, CV_BGR2GRAY);
    } else if (img.channels() == 4) {
      cv::cvtColor(img, gray_image, CV_BGRA2GRAY);
    } else {
      gray_image = img;
    }
    if (gray_image.cols != track_params_.ive_input_img_width || gray_image.rows != track_params_.ive_input_img_height) {
      cv::resize(gray_image, resized_gray_image, cv::Size(track_params_.ive_input_img_width, track_params_.ive_input_img_height ));
    } else {
      resized_gray_image = gray_image;
    }
    return resized_gray_image;
}

cv::Mat IVEObjectTracker::convertYUVMat(const cv::Mat &img) {
  cv::Mat resized_yuv_image;
  if (img.channels() == 3 || img.channels() == 4) {
    // if BGR or BGRA image, resize, and then change it to yuvsp420
    cv::Mat resized_image;
    if (img.cols != track_params_.ive_input_img_width || img.rows != track_params_.ive_input_img_height) {
      cv::resize(img, resized_image, cv::Size(track_params_.ive_input_img_width, track_params_.ive_input_img_height));
    } else {
      resized_image = img;
    }
    if (resized_image.channels() == 3) {
      cv::cvtColor(resized_image, resized_yuv_image, CV_BGR2YUV_I420);
    } else {
      cv::cvtColor(resized_image, resized_yuv_image, CV_BGRA2YUV_I420);
    }
  } else if (img.channels() == 1) {
    // if single channel, then we consider it as a yuvsp420 image, use it directly
    resized_yuv_image = img;
  } else {
    // other situations: wrong format
    std::cerr << "[of_object_tracker::update]: wrong image format" << std::endl;
  }
  return resized_yuv_image;
}


#endif // if def AARCH64_HIMIX100 or AARCH64_HIMIX200

//#endif // ifdef __arm__
