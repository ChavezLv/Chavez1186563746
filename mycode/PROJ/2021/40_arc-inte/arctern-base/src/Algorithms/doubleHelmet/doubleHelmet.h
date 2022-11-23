/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         leizhiming
 *  Last modified:  2021.12.01
 *  email:          lei.zhiming@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_DOUBLEHELMET_DOUBLEHELMET_H_
#define SRC_ALGORITHMS_DOUBLEHELMET_DOUBLEHELMET_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 双人双盔运行参数类
class ARCTERN_EXPORT_API DoubleHelmetRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  DoubleHelmetRunParameter();

  /// \brief 析构函数
  ~DoubleHelmetRunParameter();
};


/// \brief 单图片双人双盔信息
using DoubleHelmetClassify = std::pair<int, float>;

/// \brief 双人双盔结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API DoubleHelmetResult : public Result {
 public:
  /// \brief 构造函数
  DoubleHelmetResult();

  /// \brief 析构函数
  ~DoubleHelmetResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { m_vClassifyResults.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { m_vClassifyResults.resize(n); }

 public:
  std::vector<DoubleHelmetClassify> m_vClassifyResults;  ///<  结果容器
};

/// \brief 双人双盔算法类
class ARCTERN_EXPORT_API DoubleHelmet : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  DoubleHelmet();

  /// \brief 析构函数
  ~DoubleHelmet() override;

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
#endif  // SRC_ALGORITHMS_DOUBLEHELMET_DOUBLEHELMET_H_

