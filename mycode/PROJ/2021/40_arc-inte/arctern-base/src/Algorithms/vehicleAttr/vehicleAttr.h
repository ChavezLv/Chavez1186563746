/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_VEHICLEATTR_VEHICLEATTR_H_
#define SRC_ALGORITHMS_VEHICLEATTR_VEHICLEATTR_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 机动车属性运行参数类
class ARCTERN_EXPORT_API VehicleAttrRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  VehicleAttrRunParameter();

  /// \brief 析构函数
  ~VehicleAttrRunParameter();

};

/// \brief 机动车颜色
const std::vector<std::string> carColor = {
    "白色","橙色","粉色","黑色","红色","黄色","灰色",
    "金色","蓝色","绿色","青色","银色","紫色","棕色"
};

/// \brief 机动车类型
const std::vector<std::string> carType = {
    "出租车","大型货车","大型客车","公交车","轿车","皮卡车",
    "轻客","商务车","微面","小型货车","小型客车","越野车"
};

/// \brief 一辆机动车的属性
typedef struct {
  std::string vehicleBrand;  ///< 机动车品牌
  float vehicleBrandScore;   ///< 机动车品牌评分
  std::string vehicleColor;  ///< 机动车颜色
  float vehicleColorScore;   ///< 机动车颜色评分
  std::string vehicleType;   ///< 机动车类型
  float vehicleTypeScore;    ///< 机动车类型评分
} SingleVehicleAttr;

/// \brief 机动车属性结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API VehicleAttrResult : public Result {
 public:
  /// \brief 构造函数
  VehicleAttrResult();

  /// \brief 析构函数
  ~VehicleAttrResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { vehicleAttrs_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { vehicleAttrs_.resize(n); }

 public:
  std::vector<SingleVehicleAttr> vehicleAttrs_;  ///<  结果容器
};

/// \brief 机动车属性算法类
class ARCTERN_EXPORT_API VehicleAttr : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  VehicleAttr();

  /// \brief 析构函数
  ~VehicleAttr() override;

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
#endif  // SRC_ALGORITHMS_VEHICLEATTR_VEHICLEATTR_H_

