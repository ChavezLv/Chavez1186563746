/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.04
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#pragma once
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {

class AlgorithmBase;

struct DOG_DETECT_TINY_INFO {
  ArcternRect rect; //检测框
  float score;      //可信度得分
};

using DOG_DETECT_INFOS = std::vector<DOG_DETECT_TINY_INFO>;

/// \brief 人脸检测运行参数类
class ARCTERN_EXPORT_API dogDetTinyRunParameter final : public RunParameter {
 public:
  /// \brief 构造函数
  dogDetTinyRunParameter();

  /// \brief 析构函数
  ~dogDetTinyRunParameter() final;

  /// \brief 检查函数
  ErrorType CheckSize() const override;
};

class ARCTERN_EXPORT_API dogDetTinyResult : public Result {
public:
  dogDetTinyResult();
  ~dogDetTinyResult() override;

  void Release() override { detInfos_.clear(); }

  void resize(int n) override { detInfos_.resize(n); }

public:
  std::vector<DOG_DETECT_INFOS> detInfos_; ///<  检测信息
};

class ARCTERN_EXPORT_API dogDetTiny : public AlgorithmInterface {
public:
  dogDetTiny();
  ~dogDetTiny() override;

  ErrorType Init(const InitParameter *p) override;

  void Release() override;

  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *impl_; // = nullptr;    ///< 算法实现类
};
} // namespace arctern
