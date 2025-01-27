/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.26
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_MULTIDETECTCENTER_MULTIDETECTCENTER_H_
#define SRC_ALGORITHMS_MULTIDETECTCENTER_MULTIDETECTCENTER_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

enum class MultiCatType : int {
  BIKE        = 0,   ///< 非机动车
  CAR         = 1,   ///< 汽车
  LICENSE     = 2,   ///< 车牌
  HUMAN       = 3,   ///< 人
  FACE        = 4,   ///< 人脸
  HEAD_HUMAN  = 5,   ///< 人头
};

/// \brief  单物体检测信息
typedef struct {
  ArcternRect rect;   ///< 检测框
  float score;        ///< 检测可信度得分
  MultiCatType type;  ///< 物体类型
  float quality;      ///< 图像质量（0-1）
} MULTI_DETECT_CENTER_INFO;

using DETECT_INFOS = std::vector<MULTI_DETECT_CENTER_INFO>;

/// \brief 物体检测运行参数类
class ARCTERN_EXPORT_API MultiDetectCenterRunParameter final : public RunParameter {
 public:
  /// \brief 构造函数
  MultiDetectCenterRunParameter();

  /// \brief 析构函数
  ~MultiDetectCenterRunParameter() final;

  /// \brief 检查函数
  ErrorType CheckSize() const override;
};

/// \brief 物体检测结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API MultiDetectCenterResult : public Result {
 public:
  /// \brief 构造函数
  MultiDetectCenterResult();

  /// \brief 析构函数
  ~MultiDetectCenterResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { detInfos_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { detInfos_.resize(n); }

 public:
  std::vector<DETECT_INFOS> detInfos_;   ///<  检测信息
};

/// \brief 多物体检测算法类
class ARCTERN_EXPORT_API MultiDetectCenter : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  MultiDetectCenter();

  /// \brief 析构函数
  ~MultiDetectCenter() override;

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
}  /// namespace arctern
#endif  // SRC_ALGORITHMS_MULTIDETECTCENTER_MULTIDETECTCENTER_H_
