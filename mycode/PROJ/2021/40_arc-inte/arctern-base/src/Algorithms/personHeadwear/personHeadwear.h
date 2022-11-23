/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_PERSONHEADWEAR_PERSONHEADWEAR_H_
#define SRC_ALGORITHMS_PERSONHEADWEAR_PERSONHEADWEAR_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人体头戴物运行参数类
class ARCTERN_EXPORT_API PersonHeadwearRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  PersonHeadwearRunParameter();

  /// \brief 析构函数
  ~PersonHeadwearRunParameter();
};

#ifndef PERSONHEADWEARTYPE
#define PERSONHEADWEARTYPE
/// \brief 头戴物类型
enum class PersonHeadwearType : int {
  HELMET   = 0,   ///<  头盔
  HAT       = 1,   ///<  帽子
  NONE      = 2,   ///<  无头戴物
  OTHERTYPE = 3,   ///<  other
};
#endif

/// \brief 单人头戴物信息
typedef struct {
  PersonHeadwearType headwearType;   ///< 状态
  float typeScore;     ///< 状态得分
} SinglePersonHeadwear;

/// \brief 人体头戴物结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API PersonHeadwearResult : public Result {
 public:
  /// \brief 构造函数
  PersonHeadwearResult();

  /// \brief 析构函数
  ~PersonHeadwearResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { PersonHeadwears_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { PersonHeadwears_.resize(n); }

 public:
  std::vector<SinglePersonHeadwear> PersonHeadwears_;  ///<  结果容器
};

/// \brief 人体头戴物算法类
class ARCTERN_EXPORT_API PersonHeadwear : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  PersonHeadwear();

  /// \brief 析构函数
  ~PersonHeadwear() override;

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
#endif  // SRC_ALGORITHMS_PERSONHEADWEAR_PERSONHEADWEAR_H_

