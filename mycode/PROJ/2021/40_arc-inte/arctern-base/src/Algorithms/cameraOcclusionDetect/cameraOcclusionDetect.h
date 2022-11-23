/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_CAMERAOCCLUSIONDETECT_CAMERAOCCLUSIONDETECT_H_
#define SRC_ALGORITHMS_CAMERAOCCLUSIONDETECT_CAMERAOCCLUSIONDETECT_H_
#include <vector>
#include <memory>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 相机遮挡检测与背景重构算法的参数配置
typedef struct {
  int s_param = 19;                  ///< how many pixel to judge the static status
  int n_param = 3;                   ///< param that decide the region of calcing motion quantities
  int resize_width = 480;            ///< input image width
  int resize_height = 320;           ///< input image height
  int x_blocks = 2;                  ///< blocks in x direction for calcing motion
  int y_blocks = 2;                  ///< blocks in y direction for calcing motion
  int least_frame_count = 90;        ///< least frame cout for background estimation
  float motion_thr = 0.3;           ///< motion thr
  float regionmotion_thr = 0.3;     ///< motion thr
  int lessmotioncount_thr = 2;       ///< no motion blocks thr
  int static_thr = 12;               ///< no motion continus frames thr
  float ssim_thr = 0.8f;             ///< ssim value of generated background and Ground Truth background
} CameraOcclusionDetectInitParameter;

/// \brief 相机遮挡检测与背景重构运行参数类
class ARCTERN_EXPORT_API CameraOcclusionDetectRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  CameraOcclusionDetectRunParameter();

  /// \brief 析构函数
  ~CameraOcclusionDetectRunParameter();

  std::vector<ArcternImage> backgroundRefsV_;
};

/// \brief 相机遮挡检测与背景重构结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API CameraOcclusionDetectResult : public Result {
 public:
  /// \brief 构造函数
  CameraOcclusionDetectResult();

  /// \brief 析构函数
  ~CameraOcclusionDetectResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override {
    backgroundImageReal.clear(); 
    backgroundImagePred.clear(); 
  }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override {;}

 public:
  std::vector<unsigned char> backgroundImageReal; ///< 干净的背景图片，用于后续重构背景的对比
  std::vector<unsigned char> backgroundImagePred; ///< 根据连续视频帧重构的背景图像
  bool isOcclusion;  ///< 是否被遮挡
  float similarity;  ///< 重构背景与目标背景的相似度
};

/// \brief 相机遮挡检测与背景重构算法类
class ARCTERN_EXPORT_API CameraOcclusionDetect {
 public:
  /// \brief 构造函数
  CameraOcclusionDetect();

  /// \brief 析构函数
  ~CameraOcclusionDetect();

  /// \brief 算法初始化
  /// \param p - 初始化参数
  /// \return 错误码
  ErrorType Init(CameraOcclusionDetectInitParameter *p);

  /// \brief 释放资源
  void Release();

  /// \brief 重构当前摄像头前干净的背景图片 backgroundImageReal, 相机安装好之后执行一次即可。
  /// \param p - 运行参数
  /// \return 算法结果
  const Result *GetRealBackground(const RunParameter *p);
  
  /// \brief 预测相机是否被遮挡
  /// \param p - 运行参数
  /// \return 算法结果
  ErrorType Process(const RunParameter *p,Result *r);


  Result *result_ = nullptr;
 private:
  class Impl;
  std::shared_ptr<Impl> impl_; ///< 算法实现类
};

}  // namespace arctern
#endif  // SRC_ALGORITHMS_CAMERAOCCLUSIONDETECT_CAMERAOCCLUSIONDETECT_H_

