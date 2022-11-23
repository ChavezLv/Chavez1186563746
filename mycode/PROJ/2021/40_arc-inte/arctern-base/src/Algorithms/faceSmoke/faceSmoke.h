/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACESMOKE_FACESMOKE_H_
#define SRC_ALGORITHMS_FACESMOKE_FACESMOKE_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 抽烟运行参数类
class ARCTERN_EXPORT_API FaceSmokeRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceSmokeRunParameter();

  /// \brief 析构函数
  ~FaceSmokeRunParameter() override;
};

/// \brief  抽烟状态
typedef enum {
  NOSmoke = 0,  ///< 没有抽烟
  Smoke = 1,  ///<  正在抽烟
} SmokeStatus;

/// \brief  单人脸抽烟信息
typedef struct {
  SmokeStatus smokeStatus;   ///<  抽烟状态
  float statusScore;         ///<  抽烟状态可信度得分
} SingleFaceSmoke;

/// \brief 抽烟结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceSmokeResult : public Result {
 public:
  /// \brief 构造函数
  FaceSmokeResult();

  /// \brief 析构函数
  ~FaceSmokeResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceSmokes_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceSmokes_.resize(n); }

 public:
  std::vector<SingleFaceSmoke> faceSmokes_;  ///< 结果容器
};

/// \brief 抽烟算法类
class ARCTERN_EXPORT_API FaceSmoke : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceSmoke();

  /// \brief 析构函数
  ~FaceSmoke() override;

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
#endif  // SRC_ALGORITHMS_FACESMOKE_FACESMOKE_H_
