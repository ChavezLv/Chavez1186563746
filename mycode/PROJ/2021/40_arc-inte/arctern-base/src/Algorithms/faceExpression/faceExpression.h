/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEEXPRESSION_FACEEXPRESSION_H_
#define SRC_ALGORITHMS_FACEEXPRESSION_FACEEXPRESSION_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 人脸表情运行参数类
class ARCTERN_EXPORT_API FaceExpressionRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceExpressionRunParameter();

  /// \brief 析构函数
  ~FaceExpressionRunParameter() override ;
};

/// \brief 表情类型
typedef enum {
  ANGRY = 0,     ///< 愤怒
  DISGUST = 1,   ///< 厌恶
  FEAR = 2,      ///< 害怕
  HAPPY = 3,     ///< 高兴
  SAD = 4,       ///< 悲伤
  SURPRISE = 5,  ///< 惊讶
  NEUTRAL_EXP = 6,  ///< 中性
} ExpressionType;

/// \brief 单人脸表情信息
typedef struct {
  ExpressionType type;            ///<  表情类型
  float score;                    ///<  可信度得分
} SingleFaceExpression;

/// \brief 表情结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceExpressionResult : public Result {
 public:
  /// \brief 构造函数
  FaceExpressionResult();

  /// \brief 析构函数
  ~FaceExpressionResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceExpressions_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceExpressions_.resize(n); }

 public:
  std::vector<SingleFaceExpression> faceExpressions_;  ///< 结果容器
};

/// \brief 表情算法类
class ARCTERN_EXPORT_API FaceExpression : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceExpression();

  /// \brief 析构函数
  ~FaceExpression() override;

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
#endif  // SRC_ALGORITHMS_FACEEXPRESSION_FACEEXPRESSION_H_
