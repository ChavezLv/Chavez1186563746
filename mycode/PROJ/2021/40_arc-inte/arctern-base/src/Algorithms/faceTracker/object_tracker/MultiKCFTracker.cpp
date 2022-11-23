/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "src/Algorithms/faceTracker/object_tracker/MultiKCFTracker.h"
#include <string>

using cv::Rect;
using cv::Mat;

MultiKCFTracker::MultiKCFTracker() {
  m_rtPrev = NULL;
  m_rtCur = NULL;
  kcf_rtCur = NULL;
  m_using_flag = 1;

  // mdwTimeoutFade=0;
  m_no_detect_num = 0;

  bool HOG = true;
  bool FIXEDWINDOW = false;
  bool MULTISCALE = true;
//    bool SILENT=true;
  bool LAB = false;

  // create a tracker object
  KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
}

MultiKCFTracker::~MultiKCFTracker() {
  if (m_rtPrev != NULL) {
    delete m_rtPrev;
    m_rtPrev = NULL;
  }
  if (m_rtCur != NULL) {
    delete m_rtCur;
    m_rtCur = NULL;
  }

  m_mtPrev.release();

  m_ObjInfo.m_vecFrameID.clear();
  m_ObjInfo.m_vecTime.clear();
  m_ObjInfo.m_vecTraj.clear();

  // delete the tracker
/*if(tracker != NULL){
    delete tracker;
   }
*/
}

Rect *MultiKCFTracker::GetCurPos() const {
  return m_rtCur;
}

Rect *MultiKCFTracker::GetPrevPos() const {
  return m_rtPrev;
}

// function:initialize the tracker
// tracker->init(m_mtPrev,*m_rtPrev);
void MultiKCFTracker::ResetTracker(const Rect &rect,
                                   int nID,
                                   const cv::Mat &rMatCur,
                                   float confidence,
                                   int detect_plus) {
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

  /*if(tracker!=NULL)
  {
      delete tracker;
      tracker=Tracker::create("KCF");
  }
  */
  tracker.init(rect, rMatCur);
  m_mtPrev = rMatCur;

  //  m_relate_valid_cnt = 80;
  m_confidence = confidence;
  if (detect_plus == 0) {
    m_detect_cnt = 1;
  } else {
    m_detect_cnt += 1;
  }
}

void MultiKCFTracker::Process(const Mat &matCur, const Mat &matColor, int nFrameID) {
  // cout<<"process-----"<<endl;
  if (m_mtPrev.empty()) {
    std::cout << "m_mtPrev is empty!!!" << std::endl;
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
  // tracker->update(matColor,*kcf_rtCur);
  *m_rtCur = tracker.update(matColor);
  // add tracking failed
  if (m_rtCur != NULL) {
    float max_width = 0.0;
    float max_height = 0.0;
    max_width = matColor.cols;
    max_height = matColor.rows;

    if ((*m_rtCur).x >= 0 && (*m_rtCur).y >= 0 && (*m_rtCur).width > 1 && (*m_rtCur).height > 1 \
  && (*m_rtCur).x + (*m_rtCur).width - 1 < max_width && (*m_rtCur).y + (*m_rtCur).height < max_height - 1) {
      *m_rtPrev = *m_rtCur;
      m_mtPrev = matCur;
    }
  }

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

void MultiKCFTracker::SetNoUse() {
  m_using_flag = 0;
  // cout<<"setnouse"<<endl;
  if (m_rtPrev != NULL) {
    delete m_rtPrev;
    m_rtPrev = NULL;
  }

  if (m_rtCur != NULL) {
    delete m_rtCur;
    m_rtCur = NULL;
  }

/*if (tracker!=NULL)
  {
    delete tracker;
  }
*/
  m_need_reinit_frame_rect_flag = true;
  m_start_frame_id = 0;
  //  cout<<"set no use end"<<endl;
}

int MultiKCFTracker::GetLostNum() {
  return m_lost_num;
}

float MultiKCFTracker::GetConfidence() {
  return m_confidence;
}

int MultiKCFTracker::GetDetectCnt() {
  return m_detect_cnt;
}

Mat MultiKCFTracker::GetBestFrame() {
  return m_ObjInfo.m_frame_rect.frame;
}

Rect MultiKCFTracker::GetBestRect() {
  return m_ObjInfo.m_frame_rect.rect;
}

float MultiKCFTracker::GetBestQuality() {
  return m_ObjInfo.m_frame_rect.quality;
}

int MultiKCFTracker::GetBestFrameNum() {
  return m_ObjInfo.m_frame_rect.frame_num;
}

void MultiKCFTracker::InitCandiFrameRect() {
  m_no_detect_num = 0;
  m_ObjInfo.m_frame_rect.quality = -1.0;
  m_ObjInfo.m_frame_rect.null_save_flag = false;
}

void MultiKCFTracker::UpDateCandiFrameRect(bool only_detect_flag,
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

void MultiKCFTracker::UpDateCandiFrameRectInforced(bool only_detect_flag,
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

bool MultiKCFTracker::GetNeedSaveFlag() {
  return m_need_save_flag;
}

void MultiKCFTracker::SetNeedSaveFlag(bool flag) {
  m_need_save_flag = flag;
}

int MultiKCFTracker::GetStartFrameID() {
  return m_start_frame_id;
}

void MultiKCFTracker::SetStartFrameID(int id) {
  m_start_frame_id = id;
}

bool MultiKCFTracker::GetNeedReInitFrameRectFlag() {
  return m_need_reinit_frame_rect_flag;
}

void MultiKCFTracker::SetNeedReInitFrameRectFlag(bool flag) {
  m_need_reinit_frame_rect_flag = flag;
}

void MultiKCFTracker::SaveCandiFrameRect(std::string dest_path, std::string dest_show_path) {
  char face_name[200];
  // sprintf(face_name, "%d_%d_%d_%.4f.png", m_ObjInfo.m_nID, m_ObjInfo.m_frame_rect.detect_flag,
  // m_ObjInfo.m_frame_rect.frame_num, m_ObjInfo.m_frame_rect.quality);
  sprintf(face_name,                                // NOLINT
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
