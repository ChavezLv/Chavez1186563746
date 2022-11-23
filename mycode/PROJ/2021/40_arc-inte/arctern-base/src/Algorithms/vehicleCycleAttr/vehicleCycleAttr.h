/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_VEHICLECYCLEATTR_VEHICLECYCLEATTR_H_
#define SRC_ALGORITHMS_VEHICLECYCLEATTR_VEHICLECYCLEATTR_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 非机动车属性运行参数类
class ARCTERN_EXPORT_API VehicleCycleAttrRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  VehicleCycleAttrRunParameter();

  /// \brief 析构函数
  ~VehicleCycleAttrRunParameter();

};

/// \brief 非机动车类型
enum class VehicleNoMotorType : int {
  BIKE          = 0,  ///< 自行车
  ELECTRICBIKE  = 1,  ///< 电动车/摩托车
  TRICYCLE      = 2,  ///< 三轮车
  OTHERS        = 3,  ///< 其他
};

/// \brief 非机动车朝向
enum class VehicleCycleOrientationType : int {
  FRONT    = 0,  ///< 向前
  BACK     = 1,  ///< 向后
  OTHERS   = 2,  ///< 其他
};

/// \brief 一辆非机动车的属性
typedef struct {
  VehicleNoMotorType          vehicleNoMotorType;      ///< 类型
  float                       vehicleNoMotorTypeScore; ///< 类型评分
  VehicleCycleOrientationType orientationType;         ///< 朝向
  float                       orientationTypeScore;    ///< 朝向评分
  bool                        hasUmbrella;             ///< 是否安装遮阳伞/车篷
  float                       hasUmbrellaScore;        ///< 车篷预测评分
  bool                        isCycling;               ///< 是否处于骑行中
  float                       isCyclingScore;          ///< 骑行状态预测评分
  bool                        isMultiPerson;           ///< 是否载人
  float                       isMultiPersonScore;      ///< 载人预测评分
} SingleVehicleCycleAttr;

/// \brief 非机动车属性结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API VehicleCycleAttrResult : public Result {
 public:
  /// \brief 构造函数
  VehicleCycleAttrResult();

  /// \brief 析构函数
  ~VehicleCycleAttrResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { vehicleCycleAttrs_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { vehicleCycleAttrs_.resize(n); }

 public:
  std::vector<SingleVehicleCycleAttr> vehicleCycleAttrs_;  ///<  结果容器
};

/// \brief 非机动车属性算法类
class ARCTERN_EXPORT_API VehicleCycleAttr : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  VehicleCycleAttr();

  /// \brief 析构函数
  ~VehicleCycleAttr() override;

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
#endif  // SRC_ALGORITHMS_VEHICLECYCLEATTR_VEHICLECYCLEATTR_H_

