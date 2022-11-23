/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MULTIKCFTRACKER_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MULTIKCFTRACKER_H_
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

#include "opencv2/opencv.hpp"
#include "./timeEvaluate.h"
#include "./utilsfunction.h"
#include "./trackingPC.h"
#include "./kcftracker.hpp"
#include "./SingleTracker.h"

class MultiKCFTracker : public SingleTracker {
  struct frame_rect_candidate {
    bool null_save_flag;
    int frame_num;
    int detect_flag;
    float pose_roll;
    float pose_yaw;
    float pose_pitch;
    float quality;
    cv::Mat frame;
    cv::Rect rect;
  };

  class TargetInfo {
   public:
    int m_nID;
    std::vector<cv::Rect> m_vecTraj;
    std::vector<struct tm> m_vecTime;
    std::vector<int> m_vecFrameID;
    frame_rect_candidate m_frame_rect;
    TargetInfo() : m_nID(-1) {}
  };

 public:
  typedef enum {
    FACE_COUNT_ACTION_TYPE_INC = 0,
    FACE_COUNT_ACTION_TYPE_RST = 1,
  } EFaceCountActionType;

  MultiKCFTracker();
  ~MultiKCFTracker();

  void ResetTracker(const cv::Rect &rect, int nID, const cv::Mat &rMatCur, float confidence, int detect_plus);
  void SetNoUse();
  int GetLostNum();
  float GetConfidence();
  int GetDetectCnt();
  cv::Mat GetBestFrame();
  cv::Rect GetBestRect();
  float GetBestQuality();
  int GetBestFrameNum();
  bool GetNeedSaveFlag();
  void SetNeedSaveFlag(bool);
  int GetStartFrameID();
  void SetStartFrameID(int);
  bool GetNeedReInitFrameRectFlag();
  void SetNeedReInitFrameRectFlag(bool);

  void Process(const cv::Mat &matCur, const cv::Mat &matColor, int nFrameID);

  cv::Rect *GetCurPos() const;
  cv::Rect *GetPrevPos() const;

  bool ValidateResult(int nX,
                      int nY,
                      int nWidth,
                      int nHeight,
                      const cv::Mat &rMatCur,
                      const cv::Mat &rMatColor);

  int TimeoutFadeUpdate(int dwUpdateType);
  void InitCandiFrameRect();
/// @brief update candidate frame rect , if input quality is bigger than exist
  void UpDateCandiFrameRect(bool only_detect_flag,
                            int frame_num,
                            int detect_flag,
                            float pose_roll,
                            float pose_yaw,
                            float pose_pitch,
                            float quality,
                            cv::Mat frame,
                            cv::Rect rect);
  /// @brief update candidate frame rect without condition
  void UpDateCandiFrameRectInforced(bool only_detect_flag,
                                    int frame_num,
                                    int detect_flag,
                                    float pose_roll,
                                    float pose_yaw,
                                    float pose_pitch,
                                    float quality,
                                    cv::Mat frame,
                                    cv::Rect rect);

  void SaveCandiFrameRect(std::string dest_path, std::string dest_show_path);

 public:
  static float m_fFBFaliureThre;
  static float m_fFBResetThre;

  TargetInfo m_ObjInfo;
  int m_using_flag;
  int m_no_detect_num;
  int m_r;
  int m_v;

 private:
  KCFTracker tracker;
  // Ptr<Tracker> tracker;
  // member
  cv::Mat m_mtPrev;
  cv::Mat m_mtCur;
  cv::Rect *m_rtPrev;
  cv::Rect *m_rtCur;
  cv::Rect2d *kcf_rtCur;
  cv::Mat matColor;

  int m_lost_num = 0;
  int m_lost_num_thre = 1;
  cv::Mat matTemplateROI;

  float m_scale = 1.0;
  int m_relate_valid_cnt = 0;
  float m_confidence = 0;  // for overlap tracker killing, keep high confidence tracker
  int m_detect_cnt = 0;
  bool m_need_save_flag = false;  // save tracker info flag
  bool m_need_reinit_frame_rect_flag = false;  // re init tracker best frame rect quality flag
  int m_start_frame_id = 0;  // start frame id, for muliti-extracting in one tracking proecess
};
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_MULTIKCFTRACKER_H_
