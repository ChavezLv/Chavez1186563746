/// @brief simple lucas knade optical flow tracker
/// Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
/// @author Zhang Zhaofeng
/// @date 2016.10.20

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_SLKTRACKER_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_SLKTRACKER_H_

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "SingleTracker.h"
#include "./utilsfunction.h"
#include "trackingPC.h"

class SLKTracker : public SingleTracker {
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

  SLKTracker(int dwFunctionEn, int dwResizeMul, int dwResizeDiv);
  ~SLKTracker();

  void GenGridPoints();
  float NCCMetric(cv::Rect *rectCur, const cv::Mat &matCur);

  void MatrixPredict(cv::Rect *&rectCur,
                     const std::vector<cv::Point2f> &pointsPrev,
                     const std::vector<cv::Point2f> &pointsCur,
                     const cv::Rect *rectPrev,
                     const cv::Mat &rMatCur,
                     const cv::Mat &rMatColor);

  void PairwiseDistance(std::vector<float> &vecDist, const std::vector<cv::Point2f> &vecPoint);
  void ResetTracker(const cv::Rect &rect, int nID, const cv::Mat &rMatCur, float confidence, int detect_plus);
  void SetNoUse();
  // @zzf modified 161005
  int ValidatRelation(int i);
  int GetLostNum();
  float GetNccVal();
  float GetSacleVal();
  float GetRelateValidCnt();
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

  // void Process(const Mat& matCur);
  // void Process(const Mat& matCur, const Mat& matColor);
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
// @brief update candidate frame rect , if input quality is bigger than exist
  void UpDateCandiFrameRect(bool only_detect_flag,
                            int frame_num,
                            int detect_flag,
                            float pose_roll,
                            float pose_yaw,
                            float pose_pitch,
                            float quality,
                            cv::Mat frame,
                            cv::Rect rect);
// @brief update candidate frame rect without condition
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
  // grid points
  static const int m_nXCount = 10;  // orig is 10, zzf_modified
  static const int m_nYCount = 10;  // orig is 10, zzf_modified
  static const int m_nMarginX = 5;
  static const int m_nMarginY = 5;

  // pyramid LK
  static const int m_nMaxLevelLK = 5;
  const cv::Size m_sizeWinLK;
  const double m_dbDerivLambdaLK;
  const cv::TermCriteria m_criteriaLK;

  // NCC
  static const int m_nTemplateNCC = CV_TM_CCOEFF_NORMED;
  const cv::Size m_sizePatchNCC;

  // parameter
  int mdwFunctionEn;
  int mdwResizeMul;
  int mdwResizeDiv;

  // member
  cv::Mat m_mtPrev;
  cv::Mat m_mtCur;
  cv::Rect *m_rtPrev;
  cv::Rect *m_rtCur;
  std::vector<cv::Point2f> m_ptPrev;
  std::vector<cv::Point2f> m_ptCur;
  std::vector<cv::Point2f> m_ptCur2Prev;
  std::vector<int> m_nccRet;
  std::vector<int> m_fbRet;
  std::vector<unsigned char> m_forwardStatus;
  std::vector<unsigned char> m_backwardStatus;
  std::vector<float> m_fbError;
  int m_nPointsCount;

  int mdwTimeoutFade;

  cv::Mat matColor;

  // @zzf modified 161005
  int m_lost_num = 0;
  int m_lost_num_thre = 1;
  cv::Mat matTemplateROI;
  float m_retv_ncc = 0;
  int m_template_cnt = 0;
  float m_scale = 1.0;
  int m_relate_valid_cnt = 0;
  float m_confidence = 0;  // for overlap tracker killing, keep high confidence tracker
  int m_detect_cnt = 0;
  bool m_need_save_flag = false;  // save tracker info flag
  bool m_need_reinit_frame_rect_flag = false;  // re init tracker best frame rect quality flag
  int m_start_frame_id = 0;  // start frame id, for muliti-extracting in one tracking proecess
};
#endif   // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_SLKTRACKER_H_
