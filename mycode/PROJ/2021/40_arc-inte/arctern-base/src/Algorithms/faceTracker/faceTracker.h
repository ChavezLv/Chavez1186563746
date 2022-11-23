/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_FACETRACKER_H_
#define SRC_ALGORITHMS_FACETRACKER_FACETRACKER_H_

#include "include/arctern.h"

#include "./define.h"

namespace arctern {
class ObjectTracker;

/// \brief 人脸跟踪算法类
class ARCTERN_EXPORT_API FaceTracker {
 public:

  /// \brief 构造函数
  /// \param tracker_params - 跟踪参数
  explicit FaceTracker(const ObjectTrackerParameter &tracker_params);

  /// \brief 析构函数
  ~FaceTracker();

  /// \brief 注册回调函数对象
  /// \param sink - 回调对象指针
  void initDetectRecogSink(IDetectRecogSink *sink);

  /// \brief 跟踪人脸
  /// \param image - 图像桢
  /// \return 跟踪结果
  TrackResult trackFace(arctern::ArcternImage image);
 private:
  ObjectTracker *impl_;   ///< 算法实现类
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACETRACKER_FACETRACKER_H_
