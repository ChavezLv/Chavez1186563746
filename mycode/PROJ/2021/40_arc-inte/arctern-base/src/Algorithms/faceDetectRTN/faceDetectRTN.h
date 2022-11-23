/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEDETECTRTN_FACEDETECTRTN_H_
#define SRC_ALGORITHMS_FACEDETECTRTN_FACEDETECTRTN_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief  单人脸检测信息
typedef struct {
  ArcternRect rect;           ///<  人脸框
  ArcternPoint landmarks[5];  ///<  人脸关键点
  float score;                ///<  人脸框可信度
  float quality;              ///<  人脸质量
} FACE_DETECT_RTN_INFO;

/// \brief 人脸检测运行参数类
class ARCTERN_EXPORT_API FaceDetectRTNRunParameter final : public RunParameter {
 public:
  /// \brief 构造函数
  FaceDetectRTNRunParameter();

  /// \brief 析构函数
  ~FaceDetectRTNRunParameter() final;

  /// \brief 检查函数
  ErrorType CheckSize() const override;
};


/// \brief 人脸检测结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceDetectRTNResult : public Result {
 public:
  /// \brief 构造函数
  FaceDetectRTNResult();

  /// \brief 析构函数
  ~FaceDetectRTNResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { detInfos_.clear(); }


  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { detInfos_.resize(n); }

 public:
  std::vector<std::vector<FACE_DETECT_RTN_INFO>> detInfos_;  ///<  检测信息
};

/// \brief 人脸检测算法类
class ARCTERN_EXPORT_API FaceDetectRTN : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceDetectRTN();

  /// \brief 析构函数
  ~FaceDetectRTN()  override;

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
#endif  // SRC_ALGORITHMS_FACEDETECTRTN_FACEDETECTRTN_H__
