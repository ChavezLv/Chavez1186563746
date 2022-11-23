#ifndef SRC_ARCTERN_OF_OBJECT_TRACKER_H
#define SRC_ARCTERN_OF_OBJECT_TRACKER_H

#if defined(AARCH64_HIMIX100) || defined(AARCH64_HIMIX200)

//#include "detector.h"
//#include "arctern_manager.h"
#include "IVELKTracker.h"
#include "IVEKCFTracker.h"
#include <memory>

extern "C" {
#include <if_algo/if_algo_api.h>
#include <if_algo/if_algo_db.h>
}

namespace arctern {

/// optical flow object tracker, there're 2 kinds of this tracker: the detection-based object tracker and non-detection-based object tracker
/// you construct a detection-based tracker with a detector (face detector, pedestrian detector, etc.), and use track(const cv::Mat &) to track continiously frames, and use get_tracking_map, get_tracked_map, get_result to get the tracking result
/// you can also construct a no-deteciton-based tracker with only ObjectTrackerParameter, and use track(const cv::Mat &, std::vector<TrackerInfo> &) to track and get results (all results writes into 2nd parameter)
class IVEObjectTracker {
public:
  /// create a detector-based optical flow object tracker, it detects at detection-frames and tracks in tracking-frames
//  IVEObjectTracker(const std::shared_ptr<Detector> &detector, const ObjectTrackerParameter &object_tracker_params, int channel_index = 0);
  /// create an optical flow object tracker
  IVEObjectTracker(const ObjectTrackerParameter &object_tracker_params, int channel_index = 0);
  /// deconstructor
  virtual ~IVEObjectTracker();
  /// track objects in continiusly image frames, it detects in detection frames and tracks in tracking frames
  void track(const cv::Mat &image);
  /// get informations about tracking objects, key: object id, value: TrackerInfo to the objects, only avaliable in detection-based tracker
  const std::map<int, TrackerInfo> &get_tracking_map();
  /// get informations about previously tracked objects, key: object id, value: TrackerInfo to the objects, only avaliable in detection-based tracker
  const std::map<int, TrackerInfo> &get_tracked_map();
  /// get informations about tracking objects AND previously tracked objects, only avaliable in detection-based tracker
  TrackResult get_result();
  /// track objects in continiusly image frames, without any detections, only avaliable in no-detection-based optical flow object tracker
  virtual std::vector<TrackerInfo> non_detection_track(const cv::Mat &image);
  /// update objects rect with an outcoming tracking_infos, only avaliable in no-detection-based optical flow object tracker
  virtual std::vector<TrackerInfo> non_detection_update(const cv::Mat &image, const std::vector<cv::Rect> &rects) ;
//  virtual void SetArcternManager(ArcternManager *arcternManager){};
protected:
  ObjectTrackerParameter track_params_;
//  std::shared_ptr<Detector> detector_;
  long current_frame_index_ = 0;
  int channel_index_; ///< index for multiple video inputs, default to 0
  // cv::Mat prev_frame_; ///< previous frame
  t_track_face_deil curr_faces_track_;
  t_track_face_deil prev_faces_track_;
  std::map<int, TrackerInfo> tracking_map_; ///< objects not being tracked, or lost, key: id, value: tracker info
  std::map<int, TrackerInfo> tracked_map_; ///< objects currently being tracked, key: id, value: tracker info
  void check_and_update_cache_(const t_track_face_deil &deil, const cv::Size &frame_size);

  IVE_LK_TRACK* ive_lk_tracker_ = NULL;///< struct of tracker
  IVE_KCF_TRACK* ive_kcf_tracker_ = NULL;///< struct of tracker
  void trans_data_to_LK(const cv::Mat &gray_image, const cv::Size &orig_size);
  void trans_data_to_KCF(const cv::Mat &gray_image, const cv::Size &orig_size);

  cv::Mat convertGrayMat(const cv::Mat & img);

  cv::Mat convertYUVMat(const cv::Mat &img);
}; // end for class IVEObjectTracker

} // end for namespace arctern

#endif // if def AARCH64_HIMIX100 or AARCH64_HIMIX200

#endif //end for ifndef SRC_ARCTERN_OF_OBJECT_TRACKER_H
