/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.04
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_PERSONCOMPLETENESS_PERSONCOMPLETENESS_H_
#define SRC_ALGORITHMS_PERSONCOMPLETENESS_PERSONCOMPLETENESS_H_

#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人体完整度运行参数类
class ARCTERN_EXPORT_API PersonCompletenessRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  PersonCompletenessRunParameter();

  /// \brief 析构函数
  ~PersonCompletenessRunParameter() override;
};

/// \brief 单人体的完整度信息
struct SinglePersonCompleteness {
  std::vector<float> truncation;  ///< 人体完整度, [上半身完整度，下半身完整度]
  std::vector<bool> visibleParts;   ///< 人体部分的可视信息，包括头、胸部、腹部、大腿、小腿
  std::vector<float> visiblePartsScore;   ///< 人体部分的可视信息的置信度
  std::vector<std::vector<bool>> softMasks;  ///< 人体分成16*8个网格，描述其是否可视
  std::vector<std::vector<float>> softMasksScore;  ///< 人体分成16*8个网格，描述其是否可视的置信度
};

/// \brief 人体完整度结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API PersonCompletenessResult : public Result {
 public:
  /// \brief 构造函数
  PersonCompletenessResult();

  /// \brief 析构函数
  ~PersonCompletenessResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { personCompleteness_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { personCompleteness_.resize(n); }

 public:
  std::vector<SinglePersonCompleteness> personCompleteness_;  ///< 结果容器
};

/// \brief 人体完整度算法类
class ARCTERN_EXPORT_API PersonCompleteness : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  PersonCompleteness();

  /// \brief 析构函数
  ~PersonCompleteness() override;

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
#endif  // SRC_ALGORITHMS_PERSONCOMPLETENESS_PERSONCOMPLETENESS_H_
