/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEBANGS_FACEBANGS_H_
#define SRC_ALGORITHMS_FACEBANGS_FACEBANGS_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 刘海运行参数类
class ARCTERN_EXPORT_API FaceBangsRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceBangsRunParameter();

  /// \brief 析构函数
  ~FaceBangsRunParameter();
};

/// \brief 人脸刘海状态
typedef enum {
  NOBANGS = 0,  ///< 无刘海
  BANGS = 1 ,   ///< 有刘海
} BangsStatus;

/// \brief 单人脸刘海状态信息
typedef struct {
  BangsStatus status;    ///< 状态
  float statusScore;     ///<  状态可信度得分
} SingleFaceBangs;

/// \brief 刘海结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceBangsResult : public Result {
 public:
  /// \brief 构造函数
  FaceBangsResult();

  /// \brief 析构函数
  ~FaceBangsResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceBangss_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceBangss_.resize(n); }

 public:
  std::vector<SingleFaceBangs> faceBangss_;  ///< 结果容器
};

/// \brief 刘海算法类
class ARCTERN_EXPORT_API FaceBangs : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceBangs();

  /// \brief 析构函数
  ~FaceBangs() override;

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
  AlgorithmBase *impl_ = nullptr;     ///< 算法实现类
};

}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEBANGS_FACEBANGS_H_
