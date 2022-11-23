/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.18
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEOCCLUSION_FACEOCCLUSION_H_
#define SRC_ALGORITHMS_FACEOCCLUSION_FACEOCCLUSION_H_
#include <vector>
#include <utility>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸遮挡运行参数类
class ARCTERN_EXPORT_API FaceOcclusionRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceOcclusionRunParameter();

  /// \brief 析构函数
  ~FaceOcclusionRunParameter();
};

using occlusionStatus = std::pair<bool, float>;   ///<  打开/关闭 ，可信度

/// \brief  单人脸人脸遮挡信息
typedef struct {
  occlusionStatus leftEyeOpen;   ///< 左眼睁开
  occlusionStatus rightEyeOpen;  ///< 右眼睁开
  occlusionStatus leftEyeOcc;    ///< 左眼遮挡
  occlusionStatus rightEyeOcc;   ///< 右眼遮挡
  occlusionStatus leftCheekOcc;  ///< 左脸颊遮挡
  occlusionStatus rightCheekOcc;  ///< 右脸颊遮挡
  occlusionStatus mouthOcc;       ///< 鼻子遮挡
  occlusionStatus noseOcc;        ///< 嘴巴遮挡
  occlusionStatus chincontourOcc;  ///< 下巴遮挡
} SingleFaceOcclusion;

/// \brief 人脸遮挡结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceOcclusionResult : public Result {
 public:
  /// \brief 构造函数
  FaceOcclusionResult();

  /// \brief 析构函数
  ~FaceOcclusionResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceOcclusion_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceOcclusion_.resize(n); }

 public:
  std::vector<SingleFaceOcclusion> faceOcclusion_;   ///< 结果容器
};

/// \brief 人脸遮挡算法类
class ARCTERN_EXPORT_API FaceOcclusion : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceOcclusion();

  /// \brief 析构函数
  ~FaceOcclusion() override;

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
#endif  // SRC_ALGORITHMS_FACEOCCLUSION_FACEOCCLUSION_H_
