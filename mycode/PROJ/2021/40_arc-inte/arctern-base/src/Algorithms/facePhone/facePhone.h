/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEPHONE_FACEPHONE_H_
#define SRC_ALGORITHMS_FACEPHONE_FACEPHONE_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 打手机运行参数类
class ARCTERN_EXPORT_API FacePhoneRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FacePhoneRunParameter();

  /// \brief 析构函数
  ~FacePhoneRunParameter() override ;
};

/// \brief  打手机状态
typedef enum {
  NOPhone = 0,  ///< 没有在打手机
  Phone = 1,  ///< 在打手机
} PhoneStatus;

/// \brief 单人脸打手机信息
typedef struct {
  PhoneStatus phoneStatus;    ///<  打手机状态
  float statusScore;          ///<  打手机状态可信度得分
} SingleFacePhone;

/// \brief 打手机结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FacePhoneResult : public Result {
 public:
  /// \brief 构造函数
  FacePhoneResult();

  /// \brief 析构函数
  ~FacePhoneResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { facePhones_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { facePhones_.resize(n); }

 public:
  std::vector<SingleFacePhone> facePhones_;  ///< 结果容器
};

/// \brief 打手机算法类
class ARCTERN_EXPORT_API FacePhone : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FacePhone();

  /// \brief 析构函数
  ~FacePhone() override;

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
#endif  // SRC_ALGORITHMS_FACEPHONE_FACEPHONE_H_
