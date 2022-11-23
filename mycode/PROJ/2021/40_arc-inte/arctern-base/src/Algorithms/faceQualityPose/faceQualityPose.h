/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEQUALITYPOSE_FACEQUALITYPOSE_H_
#define SRC_ALGORITHMS_FACEQUALITYPOSE_FACEQUALITYPOSE_H_

#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸姿态质量运行参数类
class ARCTERN_EXPORT_API FaceQualityPoseRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceQualityPoseRunParameter();

  /// \brief 析构函数
  ~FaceQualityPoseRunParameter() override ;
};

/// \brief 单人脸姿态和质量
typedef struct {
  float pitch;    ///<  俯仰角
  float yaw;      ///<  偏航角
  float roll;     ///<  翻滚角
  float quality;  ///<  人脸质量得分
}SingleFaceQualityPose;

/// \brief 脸姿态质量结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceQualityPoseResult : public Result {
 public:
  /// \brief 构造函数
  FaceQualityPoseResult();

  /// \brief 析构函数
  ~FaceQualityPoseResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceQualityPose_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceQualityPose_.resize(n); }

 public:
  std::vector<SingleFaceQualityPose> faceQualityPose_;  ///< 结果容器
};

/// \brief 脸姿态质量算法类
class ARCTERN_EXPORT_API FaceQualityPose : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceQualityPose();

  /// \brief 析构函数
  ~FaceQualityPose() override;

  /// \brief 算法初始化
  /// \param p - 初始化参数
  /// \return 错误码
  ErrorType Init(const InitParameter *p) override;

  /// \brief 释放资源
  void Release() override;

  /// \brief 执行算法
  /// \param p - 运行参数
  /// \return 算法结果
  ErrorType Process(const RunParameter *p,Result *r) override;



 private:
  AlgorithmBase *impl_ = nullptr;   ///< 算法实现类
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEQUALITYPOSE_FACEQUALITYPOSE_H_
