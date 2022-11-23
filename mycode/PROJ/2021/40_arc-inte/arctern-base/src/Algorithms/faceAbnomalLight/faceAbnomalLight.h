/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEABNOMALLIGHT_FACEABNOMALLIGHT_H_
#define SRC_ALGORITHMS_FACEABNOMALLIGHT_FACEABNOMALLIGHT_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"
namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸光照质量运行参数类
class ARCTERN_EXPORT_API FaceAbnomalLightRunParameter final : public RunParameter {
 public:
  /// \brief 构造函数
  FaceAbnomalLightRunParameter();

  /// \brief 析构函数
  ~FaceAbnomalLightRunParameter() final;
};

/// \brief 人脸光照质量
typedef enum {
  BACK = 0,      ///< 逆光
  FRONT = 1,     ///< 过曝光
  DARK = 2,      ///< 过暗
  SIDE = 3,      ///< 侧光阴阳脸
  NORMAL_LIGHT = 4,  ///< 正常
} AbnomalLightType;

/// \brief 单人脸人脸光照质量
typedef struct {
  AbnomalLightType type;   ///< 人脸光照质量类型
  float score;            ///< 人脸光照质量可信度
} SingleFaceAbnomalLight;

/// \brief 人脸光照质量结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceAbnomalLightResult : public Result {
 public:
  /// \brief 构造函数
  FaceAbnomalLightResult();

  /// \brief 析构函数
  ~FaceAbnomalLightResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceAbnomalLights_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceAbnomalLights_.resize(n); }
 public:
  std::vector<SingleFaceAbnomalLight> faceAbnomalLights_;  ///< 结果容器
};

/// \brief 人脸光照质量算法类
class ARCTERN_EXPORT_API FaceAbnomalLight : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceAbnomalLight();

  /// \brief 析构函数
  ~FaceAbnomalLight() override;

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
  AlgorithmBase *impl_ = nullptr;  ///< 算法实现类
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEABNOMALLIGHT_FACEABNOMALLIGHT_H_
