/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACELIVENESSRGB_FACELIVENESSRGB_H_
#define SRC_ALGORITHMS_FACELIVENESSRGB_FACELIVENESSRGB_H_

#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief rgb活体检测运行参数类
class ARCTERN_EXPORT_API FaceLivenessRgbRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceLivenessRgbRunParameter();

  /// \brief 析构函数
  ~FaceLivenessRgbRunParameter() override;
};

using livenessScore = float;

/// \brief rgb活体检测结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceLivenessRgbResult : public Result {
 public:
  /// \brief 构造函数
  FaceLivenessRgbResult();

  /// \brief 析构函数
  ~FaceLivenessRgbResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceLiveness_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceLiveness_.resize(n); }

 public:
  std::vector<livenessScore> faceLiveness_;  ///< 结果容器
};

/// \brief rgb活体检测算法类
class ARCTERN_EXPORT_API FaceLivenessRgb : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceLivenessRgb();

  /// \brief 析构函数
  ~FaceLivenessRgb() override;

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
#endif  // SRC_ALGORITHMS_FACELIVENESSRGB_FACELIVENESSRGB_H_
