/*
* @file SLKTracker.cpp
* @date 2016-10-20
* @brief  Simple Lucas Kanade Optical Flow Tracker
* @author ZhangZhaofeng(zhang.zhaofeng@intellif.com)
*
* Copyright (c) 2016, Shenzhen Intellifusion Technologies Co., Ltd.
*/

#include "src/Algorithms/faceTracker/object_tracker/SLKTracker.h"
#include <iostream>
#include <string>
#include <algorithm>

#include "./timeEvaluate.h"

using cv::TermCriteria;
using cv::Size;
using cv::Point2f;
using cv::Mat;
using cv::Rect;
using std::vector;
float SLKTracker::m_fFBFaliureThre = 100.0f;
float SLKTracker::m_fFBResetThre = 5.0f;

SLKTracker::SLKTracker(int dwFunctionEn, int dwResizeMul, int dwResizeDiv)
    : m_sizeWinLK(Size(4, 4)),
      m_dbDerivLambdaLK(0.5),
      m_criteriaLK(TermCriteria::COUNT + TermCriteria::EPS, 5, 0.03),
      m_sizePatchNCC(Size(10, 10)) {
  mdwFunctionEn = dwFunctionEn;
  mdwResizeMul = dwResizeMul;
  mdwResizeDiv = dwResizeDiv;

  m_rtPrev = NULL;
  m_rtCur = NULL;
  m_using_flag = 1;

  mdwTimeoutFade = 0;
  m_no_detect_num = 0;
}

SLKTracker::~SLKTracker() {
  m_ptPrev.clear();
  m_ptCur.clear();
  m_ptCur2Prev.clear();
  m_nccRet.clear();
  m_fbRet.clear();
  m_forwardStatus.clear();
  m_backwardStatus.clear();
  m_fbError.clear();

  if (m_rtPrev != NULL) {
    delete m_rtPrev;

    m_rtPrev = NULL;
  }
  if (m_rtCur != NULL) {
    delete m_rtCur;

    m_rtCur = NULL;
  }

  m_mtPrev.release();
  // m_mtCur.release();

  m_ObjInfo.m_vecFrameID.clear();
  m_ObjInfo.m_vecTime.clear();
  m_ObjInfo.m_vecTraj.clear();
}

void SLKTracker::GenGridPoints() {
  int nIncremX = (m_rtPrev->width - m_nMarginX * 2) / (m_nXCount - 1);
  int nIncremY = (m_rtPrev->height - m_nMarginY * 2) / (m_nYCount - 1);

  Point2f ptNewRect = m_rtPrev->tl();
  ptNewRect = ptNewRect + Point2f(m_nMarginX, m_nMarginY);

  for (int r = 0; r < m_nYCount; r++) {
    for (int c = 0; c < m_nXCount; c++) {
      m_ptPrev.push_back(Point2f(ptNewRect.x + c * nIncremX, ptNewRect.y + r * nIncremY));
    }
  }
}

// relationship validation @zzf_added
// position relationship keep stable is good
int SLKTracker::ValidatRelation(int i) {
  int gridX = i % m_nXCount;
  int gridY = i / m_nXCount;
  int valid_val = 0;
  bool valid_left = false;
  bool valid_right = false;
  bool valid_up = false;
  bool valid_bottom = false;
  if (gridX == 0) {
    valid_left = m_ptCur[i].x < m_ptCur[i + 2].x;
  } else {
    valid_left = m_ptCur[i].x > m_ptCur[i - 1].x;
  }

  if (gridX == m_nXCount - 1) {
    valid_right = m_ptCur[i].x > m_ptCur[i - 2].x;
  } else {
    valid_right = m_ptCur[i].x < m_ptCur[i + 1].x;
  }

  if (gridY == 0) {
    valid_up = m_ptCur[i].y < m_ptCur[i + 2 * m_nXCount].y;
  } else {
    valid_up = m_ptCur[i].y > m_ptCur[i - m_nXCount].y;
  }

  if (gridY == m_nYCount - 1) {
    valid_bottom = m_ptCur[i].y > m_ptCur[i - 2 * m_nXCount].y;
  } else {
    valid_bottom = m_ptCur[i].y < m_ptCur[i + m_nXCount].y;
  }

  if (valid_left) {
    valid_val++;
  }

  if (valid_right) {
    valid_val++;
  }

  if (valid_up) {
    valid_val++;
  }

  if (valid_bottom) {
    valid_val++;
  }

  return valid_val;
}

float SLKTracker::NCCMetric(Rect *rectCur, const Mat &matCur) {
  Mat matRet(1, 1, CV_32FC1);
  Mat matCurROI = matCur(*rectCur);
  resize(matCurROI, matCurROI, matTemplateROI.size(), 0, 0, CV_INTER_LINEAR);
  matchTemplate(matTemplateROI, matCurROI, matRet, CV_TM_CCOEFF_NORMED);
  float retv_template = (reinterpret_cast<float *>(matRet.data))[0];
  matRet.release();
  return retv_template;
}

// @zzf 使用rect尺寸及肤色进行甄别
bool SLKTracker::ValidateResult(int nX,
                                int nY,
                                int nWidth,
                                int nHeight,
                                const cv::Mat &rMatCur,
                                const cv::Mat &rMatColor) {
  cv::Rect rect_temp = cvRect(nX, nY, nWidth, nHeight);
  validate_rect(&rect_temp, rMatCur.cols, rMatCur.rows);
  float max_border = rect_temp.width > rect_temp.height ? rect_temp.width : rect_temp.height;
  float min_border = rect_temp.width < rect_temp.height ? rect_temp.width : rect_temp.height;
  float diff_rate = (max_border - min_border) / max_border;
  if (diff_rate > 0.5) {
    /*if ((nX           <  0) ||
        (nY           <  0) ||
        (nWidth <= 0) ||
        (nHeight <= 0) ||
        (nX + nWidth) >  rMatCur.cols ||
        (nY + nHeight)>  rMatCur.rows ||
        (nX != nX) ||
        (nY != nY) ||
        (nWidth != nWidth) ||
        (nHeight != nHeight))*/
    std::cout << "nX:" << nX << "nY:" << nY << "nWidth:" << nWidth << "nHeight:" << nHeight << "rMatCur.cols:"
              << rMatCur.cols << "rMatCur.rows:" << rMatCur.rows << std::endl;
    return false;
  } else {
    if (mdwFunctionEn) {
      if (mdwResizeMul && mdwResizeDiv) {
        nX = (nX * mdwResizeMul) / mdwResizeDiv;
        nY = (nY * mdwResizeMul) / mdwResizeDiv;
        nWidth = (nWidth * mdwResizeMul) / mdwResizeDiv;
        nHeight = (nHeight * mdwResizeMul) / mdwResizeDiv;
      }

      Rect rtTrack(nX, nY, nWidth, nHeight);
      int nSkin = getSkin(cv::Mat(rMatColor, rtTrack));
      double skinRatio = nSkin / static_cast<double>(rtTrack.width * rtTrack.height);

      if (skinRatio < 0.001) {
        return false;
      } else {
        return true;
      }
    } else {
      return true;
    }
  }
}

void SLKTracker::MatrixPredict(Rect *&rectCur,
                               const vector<Point2f> &pointsPrev,
                               const vector<Point2f> &pointsCur,
                               const Rect *rectPrev,
                               const cv::Mat &rMatCur,
                               const cv::Mat &rMatColor) {
  int nPoints = pointsPrev.size();

  if (nPoints <= 3) {
    rectCur = NULL;
    m_using_flag = 0;
    std::cout << m_ObjInfo.m_nID << " return null because has" << nPoints << std::endl;
    return;
  }

  if (rectPrev == NULL) {
    rectCur = NULL;
    m_using_flag = 0;
    std::cout << m_ObjInfo.m_nID << " return null because rectPrev==NULL " << std::endl;
    return;
  }

  int nDistPairs = nPoints * (nPoints - 1) / 2;
  vector<float> vecDistPrev(nDistPairs), vecDistCur(nDistPairs), vecScale(nDistPairs);
  PairwiseDistance(vecDistPrev, pointsPrev);
  PairwiseDistance(vecDistCur, pointsCur);

  // scale
  float fScale = 0.0f;
  float fScaleX = 0.0f;
  float fScaleY = 0.0f;

  for (int i = 0; i < nDistPairs; i++) {
    if (vecDistCur[i] > 0.0f) {
      vecScale[i] = vecDistCur[i] / vecDistPrev[i];
    }
  }

  sort(vecScale.begin(), vecScale.end());

  fScale = vecScale[nDistPairs / 2];
  // @zzf modified 20161005
  // to make sure the rect changed hugely
  m_scale = fScale;

  if (fScale > 3) {
    rectCur = NULL;
    std::cout << m_ObjInfo.m_nID << " return null because rectPrev==NULL " << std::endl;
    m_using_flag = 0;
    return;
  }

  if (fScale > 1.02) {
    fScale = 1.02;
  }

  fScaleX = 0.5 * (fScale - 1) * rectPrev->width;
  fScaleY = 0.5 * (fScale - 1) * rectPrev->height;

  // translation
  float fTranslationX = 0.0f;
  float fTranslationY = 0.0f;

  vector<float> vecXOffset(nPoints);
  vector<float> vecYOffset(nPoints);

  for (int i = 0; i < nPoints; i++) {
    vecXOffset[i] = pointsCur[i].x - pointsPrev[i].x;
    vecYOffset[i] = pointsCur[i].y - pointsPrev[i].y;
  }

  sort(vecXOffset.begin(), vecXOffset.end());
  sort(vecYOffset.begin(), vecYOffset.end());

  fTranslationX = vecXOffset[nPoints / 2];
  fTranslationY = vecYOffset[nPoints / 2];

  // new position
  if (rectCur != NULL) {
    delete rectCur;
  }

  int nRectX = static_cast<int>(rectPrev->x + fTranslationX - fScaleX + 0.5);
  int nRectY = static_cast<int>(rectPrev->y + fTranslationY - fScaleY + 0.5);
  int nRectWidht = static_cast<int>(rectPrev->width * fScale + 0.5);
  int nRectHeight = static_cast<int>(rectPrev->height * fScale + 0.5);

  if (ValidateResult(nRectX, nRectY, nRectWidht, nRectHeight, rMatCur, rMatColor)) {
    rectCur = new Rect(nRectX,
                       nRectY,
                       nRectWidht,
                       nRectHeight);
    validate_rect(rectCur, rMatCur.cols, rMatCur.rows);
  } else {
    rectCur = NULL;
    m_using_flag = 0;
    std::cout << m_ObjInfo.m_nID << " return null because ValidateResult Failed " << std::endl;
  }
}

void SLKTracker::PairwiseDistance(vector<float> &vecDist, const vector<Point2f> &vecPoint) {
  int nPoints = vecPoint.size();

  if (nPoints <= 1) {
    return;
  }

  int i = 0, j = 0, t = 0;

  for (i = 0; i < nPoints; i++) {
    for (j = i + 1; j < nPoints; j++) {
      vecDist[t++] = norm(vecPoint[i] - vecPoint[j]);
    }
  }
}

Rect *SLKTracker::GetCurPos() const {
  return m_rtCur;
}

Rect *SLKTracker::GetPrevPos() const {
  return m_rtPrev;
}

void SLKTracker::ResetTracker(const Rect &rect, int nID, const cv::Mat &rMatCur, float confidence, int detect_plus) {
  m_ObjInfo.m_nID = nID;
  m_using_flag = 1;
  m_no_detect_num = 0;

  if (m_rtPrev == NULL) {
    m_rtPrev = new Rect(rect);
  } else {
    *m_rtPrev = rect;
  }

  if (m_rtCur == NULL) {
    m_rtCur = new Rect(rect);
  } else {
    *m_rtCur = rect;
  }

  // @zzf modified 161005
  m_mtPrev = rMatCur;
  m_relate_valid_cnt = 80;
  m_confidence = confidence;
  if (detect_plus == 0) {
    m_detect_cnt = 1;
  } else {
    m_detect_cnt += 1;
  }
}

// void SLKTracker::Process(const Mat& matCur)
// @zzf must initial m_mtCur and m_rtPrev before use Process
void SLKTracker::Process(const Mat &matCur,
                         const Mat &matColor,
                         int nFrameID) {  // take care: maybe the matCur need to be gray(1 channel)
  m_ptPrev.clear();
  m_ptCur.clear();
  m_ptCur2Prev.clear();
  m_forwardStatus.clear();
  m_backwardStatus.clear();
  m_fbError.clear();
  m_nccRet.clear();
  m_fbRet.clear();

  // @zzf modified 161005
  if (m_mtPrev.empty()) {
    printf("warning the m_mtPrev is null!\n");
    if (m_rtCur != NULL) {
      delete m_rtCur;
      m_rtCur = NULL;
    }

    if (m_rtPrev != NULL) {
      delete m_rtPrev;
      m_rtPrev = NULL;
    }
    m_using_flag = 0;
  }

  // generate grid points
  mfxTime timeGPStart;
  mfxGetTime(&timeGPStart);
  GenGridPoints();
  mfxTime timeGPStop;
  mfxGetTime(&timeGPStop);
/// double timeDiffGP = TimeDiffMsec(timeGPStop, timeGPStart);
  // printf("generate grid points time is %f\n", timeDiffGP);

  m_nPointsCount = m_ptPrev.size();
  m_ptPrev.resize(m_nPointsCount);
  m_ptCur.resize(m_nPointsCount);
  m_ptCur2Prev.resize(m_nPointsCount);
  m_forwardStatus.resize(m_nPointsCount);
  m_backwardStatus.resize(m_nPointsCount);
  m_fbError.resize(m_nPointsCount);
  m_nccRet.resize(m_nPointsCount);
  m_fbRet.resize(m_nPointsCount);

  // single-direction pyramid LK computing
  // cout<<"calcOpticalFlowPyrLKPC before"<<std::endl;
  ifcv::calcOpticalFlowPyrLKPC(m_mtPrev,
                               matCur,
                               m_ptPrev,
                               m_ptCur,
                               m_forwardStatus,
                               m_fbError,
                               *m_rtPrev,
                               m_sizeWinLK,
                               m_nMaxLevelLK,
                               m_criteriaLK,
                               1);
  // cout<<"calcOpticalFlowPyrLKPC after"<<std::endl;

  // filter points
  vector<Point2f> vecCandidatePrev, vecCandidateCur;
  cv::Mat curMat = matCur;
  for (int i = 0; i < m_nPointsCount; i++) {
    if ((m_forwardStatus[i] == 1) && (ValidatRelation(i) >= 3)) {
      vecCandidatePrev.push_back(m_ptPrev[i]);
      vecCandidateCur.push_back(m_ptCur[i]);
    }
  }
  // printf("the vecCandidatPrev cnt is %d\n", vecCandidateCur.size());
  m_relate_valid_cnt = vecCandidateCur.size();

  /*//draw point test @zzf 20180906 modified;
  Mat mat_prev_draw = m_mtPrev.clone();
  Mat mat_cur_draw = matCur.clone();
  for(int i=0; i<vecCandidatePrev.size(); i++){
      circle(mat_prev_draw,vecCandidatePrev[i],2,Scalar(0,0,255));
  }
  for(int i=0; i<vecCandidateCur.size(); i++){
      circle(mat_cur_draw,vecCandidateCur[i],2,Scalar(0,0,255));
  }
  imshow("prev",mat_prev_draw);
  imshow("cur",mat_cur_draw);
  waitKey(1);
  */


  // prediction
  mfxTime timeMPStart;
  mfxGetTime(&timeMPStart);
  MatrixPredict(m_rtCur,
                vecCandidatePrev,
                vecCandidateCur,
                m_rtPrev,
                matCur,
                matColor);  // matColor just use in MatrixPredict
  mfxTime timeMPStop;
  mfxGetTime(&timeMPStop);
/// double timeDiffMP = TimeDiffMsec(timeMPStop, timeMPStart);
  // printf("MatrixPredict time is %f\n", timeDiffMP);


  if (m_rtCur != NULL) {
    *m_rtPrev = *m_rtCur;
  }

  vecCandidatePrev.clear();
  vecCandidateCur.clear();

  // @zzf modified 161005
  // just update while tracking is succeed.
  if (m_rtCur != NULL) {
    m_mtPrev = matCur;
    m_lost_num = 0;
  } else {
    m_lost_num++;
  }

  if (m_lost_num > m_lost_num_thre) {
    m_using_flag = 0;
    if (m_rtPrev != NULL) {
      delete m_rtPrev;
      m_rtPrev = NULL;
    }
    if (m_rtCur != NULL) {
      delete m_rtCur;
      m_rtCur = NULL;
    }
  }
  m_confidence = 0;
}

void SLKTracker::SetNoUse() {
  m_using_flag = 0;

  if (m_rtPrev != NULL) {
    delete m_rtPrev;
    m_rtPrev = NULL;
  }

  if (m_rtCur != NULL) {
    delete m_rtCur;
    m_rtCur = NULL;
  }
  m_need_reinit_frame_rect_flag = true;
  m_start_frame_id = 0;
}

// @zzf modified 161005
int SLKTracker::GetLostNum() {
  return m_lost_num;
}

// @zzf modified 161025
float SLKTracker::GetNccVal() {
  return m_retv_ncc;
}

float SLKTracker::GetSacleVal() {
  return m_scale;
}

float SLKTracker::GetRelateValidCnt() {
  return m_relate_valid_cnt;
}

float SLKTracker::GetConfidence() {
  return m_confidence;
}

int SLKTracker::GetDetectCnt() {
  return m_detect_cnt;
}

Mat SLKTracker::GetBestFrame() {
  return m_ObjInfo.m_frame_rect.frame;
}

Rect SLKTracker::GetBestRect() {
  return m_ObjInfo.m_frame_rect.rect;
}

float SLKTracker::GetBestQuality() {
  return m_ObjInfo.m_frame_rect.quality;
}

int SLKTracker::GetBestFrameNum() {
  return m_ObjInfo.m_frame_rect.frame_num;
}

int SLKTracker::TimeoutFadeUpdate(int dwUpdateType) {
  if (dwUpdateType == FACE_COUNT_ACTION_TYPE_INC) {
    mdwTimeoutFade += 0x1;
  } else {
    mdwTimeoutFade = 0;
  }

  return mdwTimeoutFade;
}

void SLKTracker::InitCandiFrameRect() {
  m_no_detect_num = 0;
  m_ObjInfo.m_frame_rect.quality = -1.0;
  m_ObjInfo.m_frame_rect.null_save_flag = false;
}
void SLKTracker::UpDateCandiFrameRect(bool only_detect_flag,
                                      int frame_num,
                                      int detect_flag,
                                      float pose_roll,
                                      float pose_yaw,
                                      float pose_pitch,
                                      float quality,
                                      cv::Mat frame,
                                      cv::Rect rect) {
  if (!only_detect_flag) {
    // so when tracker init, quality shoud be less than 0.0
    if (quality > m_ObjInfo.m_frame_rect.quality) {
      m_ObjInfo.m_frame_rect.frame_num = frame_num;
      m_ObjInfo.m_frame_rect.detect_flag = detect_flag;
      m_ObjInfo.m_frame_rect.pose_roll = pose_roll;
      m_ObjInfo.m_frame_rect.pose_yaw = pose_yaw;
      m_ObjInfo.m_frame_rect.pose_pitch = pose_pitch;
      m_ObjInfo.m_frame_rect.quality = quality;
      m_ObjInfo.m_frame_rect.frame = frame.clone();
      m_ObjInfo.m_frame_rect.rect = rect;
    }
  }
}

void SLKTracker::UpDateCandiFrameRectInforced(bool only_detect_flag,
                                              int frame_num,
                                              int detect_flag,
                                              float pose_roll,
                                              float pose_yaw,
                                              float pose_pitch,
                                              float quality,
                                              cv::Mat frame,
                                              cv::Rect rect) {
  if (!only_detect_flag) {
    m_ObjInfo.m_frame_rect.frame_num = frame_num;
    m_ObjInfo.m_frame_rect.detect_flag = detect_flag;
    m_ObjInfo.m_frame_rect.pose_roll = pose_roll;
    m_ObjInfo.m_frame_rect.pose_yaw = pose_yaw;
    m_ObjInfo.m_frame_rect.pose_pitch = pose_pitch;
    m_ObjInfo.m_frame_rect.quality = quality;
    m_ObjInfo.m_frame_rect.frame = frame.clone();
    m_ObjInfo.m_frame_rect.rect = rect;
    m_start_frame_id = frame_num;
  }
}

bool SLKTracker::GetNeedSaveFlag() {
  return m_need_save_flag;
}

void SLKTracker::SetNeedSaveFlag(bool flag) {
  m_need_save_flag = flag;
}

int SLKTracker::GetStartFrameID() {
  return m_start_frame_id;
}

void SLKTracker::SetStartFrameID(int id) {
  m_start_frame_id = id;
}

bool SLKTracker::GetNeedReInitFrameRectFlag() {
  return m_need_reinit_frame_rect_flag;
}

void SLKTracker::SetNeedReInitFrameRectFlag(bool flag) {
  m_need_reinit_frame_rect_flag = flag;
}

void SLKTracker::SaveCandiFrameRect(std::string dest_path, std::string dest_show_path) {
  char face_name[200];
  // sprintf(face_name, "%d_%d_%d_%.4f.png", m_ObjInfo.m_nID, m_ObjInfo.m_frame_rect.detect_flag,
  // m_ObjInfo.m_frame_rect.frame_num, m_ObjInfo.m_frame_rect.quality);
  sprintf(face_name,                                       // NOLINT
          "%d_%d_%d_%d_%d_%d_%.4f.png",
          m_ObjInfo.m_nID,
          m_ObjInfo.m_frame_rect.frame_num,
          m_ObjInfo.m_frame_rect.rect.x,
          m_ObjInfo.m_frame_rect.rect.y,
          m_ObjInfo.m_frame_rect.rect.width,
          m_ObjInfo.m_frame_rect.rect.height,
          m_ObjInfo.m_frame_rect.quality);
  std::string face_name_str = face_name;
  face_name_str = dest_path + face_name_str;
///    cv::Rect rect_temp = m_ObjInfo.m_frame_rect.rect;
  // for square face rect
  // float exth_scale=0.083;
  // square2rectangle(&rect_temp, exth_scale);
  // validate_rect(&rect_temp, 1920, 1080);
  // cv::Mat face_crop = m_ObjInfo.m_frame_rect.frame(rect_temp).clone();
//    std::cout << "x,y,w,h=" << m_ObjInfo.m_frame_rect.rect.x << ","
//    << m_ObjInfo.m_frame_rect.rect.y << "," << m_ObjInfo.m_frame_rect.rect.width
//    << "," << m_ObjInfo.m_frame_rect.rect.height << std::endl;
  if (m_ObjInfo.m_frame_rect.rect.x < 0 || m_ObjInfo.m_frame_rect.rect.y < 0 || m_ObjInfo.m_frame_rect.rect.width < 12
      || m_ObjInfo.m_frame_rect.rect.height < 12) {
    return;
  }
  cv::Mat face_crop = m_ObjInfo.m_frame_rect.frame(m_ObjInfo.m_frame_rect.rect).clone();
  Mat face_resize;
  cv::Size std_size = cvSize(96, 112);
  cv::resize(face_crop, face_resize, std_size);
  cv::imwrite(face_name_str, face_resize);

  std::string show_face_name_str = face_name;
  try {
    show_face_name_str = dest_show_path + show_face_name_str;
    cv::Rect rect_show = m_ObjInfo.m_frame_rect.rect;
    enlarge_rect(&rect_show, 0.25, 0.25);
    validate_rect(&rect_show, m_ObjInfo.m_frame_rect.frame.cols, m_ObjInfo.m_frame_rect.frame.rows);
    if (rect_show.x < 0 || rect_show.y < 0 || rect_show.width < 12 || rect_show.height < 12) {
      return;
    }
    cv::Mat face_crop_show = m_ObjInfo.m_frame_rect.frame(rect_show).clone();
    cv::Mat face_resize_show;
    cv::Size std_size = cvSize(144, 168);
    cv::resize(face_crop_show, face_resize_show, std_size);
    cv::imwrite(show_face_name_str, face_resize_show);
  }
  catch (cv::Exception &cv_error) {
    std::cout << "save show image error " << show_face_name_str << std::endl;
  }
}
