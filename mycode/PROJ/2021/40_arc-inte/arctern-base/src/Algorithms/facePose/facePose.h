/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEPOSE_FACEPOSE_H_
#define SRC_ALGORITHMS_FACEPOSE_FACEPOSE_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸姿态运行参数类
class ARCTERN_EXPORT_API FacePoseRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FacePoseRunParameter();

  /// \brief 析构函数
  ~FacePoseRunParameter() override;
};

/// \brief  人脸姿态等级
typedef enum {
  NEGATIVE_0_15 = 0,   ///< 角度-(0~15)
  POSITIVE_0_15 = 1,   ///< 角度+(0~15)
  NEGATIVE_15_30 = 2,  ///< 角度-(15~30)
  POSITIVE_15_30 = 3,  ///< 角度+(15~30)
  NEGATIVE_30_45 = 4,  ///< 角度-(30~45)
  POSITIVE_30_45 = 5,  ///< 角度+(30~45)
  NEGATIVE_45_60 = 6,  ///< 角度-(45~60)
  POSITIVE_45_60 = 7,  ///< 角度+(45~60)
  NEGATIVE_60_90 = 8,  ///< 角度-(60~90)
  POSITIVE_60_90 = 9,  ///< 角度+(60~90)
  OTHERANGLE = 10,     ///< 角度others
} FacePoseLevel;

/// \brief 单人脸姿态信息
typedef struct {
  FacePoseLevel pitch;   ///<  俯仰角
  FacePoseLevel yaw;     ///<  偏航角
  FacePoseLevel roll;    ///<  翻滚角
}SingleFacePose;

/// \brief 人脸姿态结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FacePoseResult : public Result {
 public:
  /// \brief 构造函数
  FacePoseResult();

  /// \brief 析构函数
  ~FacePoseResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { facePose_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { facePose_.resize(n); }

 public:
  std::vector<SingleFacePose> facePose_;  ///< 结果容器
};

/// \brief 人脸姿态算法类
class ARCTERN_EXPORT_API FacePose : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FacePose();

  /// \brief 析构函数
  ~FacePose() override;

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
  AlgorithmBase *impl_ = nullptr;    ///< 算法实现类
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEPOSE_FACEPOSE_H_
