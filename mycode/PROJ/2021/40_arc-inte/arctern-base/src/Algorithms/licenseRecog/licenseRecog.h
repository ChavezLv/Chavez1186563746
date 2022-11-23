/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_LICENSERECOG_LICENSERECOG_H_
#define SRC_ALGORITHMS_LICENSERECOG_LICENSERECOG_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 车牌识别运行参数类
class ARCTERN_EXPORT_API LicenseRecogRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  LicenseRecogRunParameter();

  /// \brief 析构函数
  ~LicenseRecogRunParameter();
};

/// \brief 车牌识别结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API LicenseRecogResult : public Result {
 public:
  /// \brief 构造函数
  LicenseRecogResult();

  /// \brief 析构函数
  ~LicenseRecogResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { LicenseRecogs_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override {
    LicenseRecogs_.resize(n);
    score_.resize(n);
  }

 public:
  std::vector<std::string> LicenseRecogs_;  ///<  结果容器
  std::vector<std::vector<float>> score_; ///< 每个字的置信度
};

/// \brief 车牌识别算法类
class ARCTERN_EXPORT_API LicenseRecog : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  LicenseRecog();

  /// \brief 析构函数
  ~LicenseRecog() override;

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
#endif  // SRC_ALGORITHMS_LICENSERECOG_LICENSERECOG_H_

