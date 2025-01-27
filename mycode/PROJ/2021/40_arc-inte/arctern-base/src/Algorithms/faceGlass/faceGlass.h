/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEGLASS_FACEGLASS_H_
#define SRC_ALGORITHMS_FACEGLASS_FACEGLASS_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 眼镜运行参数类
class ARCTERN_EXPORT_API FaceGlassRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceGlassRunParameter();

  /// \brief 析构函数
  ~FaceGlassRunParameter();
};

/// \brief 带眼镜状态
typedef enum {
  NOGLASSES = 0,  ///< 不戴眼镜
  GLASSES = 1,  ///< 戴普通眼镜
  SUNGLASSES = 2,  ///< 戴太阳镜
} GlassesStatus;

/// \brief 单人脸带眼镜状态
typedef struct {
  GlassesStatus glassesStatus;   ///< 状态
  float statusScore;             ///< 可信度得分
} SingleFaceGlass;

/// \brief 眼镜结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceGlassResult : public Result {
 public:
  /// \brief 构造函数
  FaceGlassResult();

  /// \brief 析构函数
  ~FaceGlassResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { FaceGlasss_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { FaceGlasss_.resize(n); }

 public:
  std::vector<SingleFaceGlass> FaceGlasss_;  ///< 结果容器
};

/// \brief 眼镜算法类
class ARCTERN_EXPORT_API FaceGlass : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceGlass();

  /// \brief 析构函数
  ~FaceGlass() override;

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
  AlgorithmBase *impl_ = nullptr;   ///< 算法实现类
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_FACEGLASS_FACEGLASS_H_
