/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEHEADWEAR_FACEHEADWEAR_H_
#define SRC_ALGORITHMS_FACEHEADWEAR_FACEHEADWEAR_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 头戴物运行参数类
class ARCTERN_EXPORT_API FaceHeadwearRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceHeadwearRunParameter();

  /// \brief 析构函数
  ~FaceHeadwearRunParameter() override ;
};

/// \brief 头戴物颜色
typedef enum {
  NOHEADWEARCOLOR = 0,  ///< 无头戴物
  BLACK = 1,            ///< 黑色
  WHITE = 2,            ///< 白色
  GRAY_BROWN = 3,       ///< 灰褐色
  RED = 4,              ///< 红色
  PINK = 5,             ///< 粉色
  ORANGE = 6,           ///< 橙色
  BLUE = 7,             ///< 蓝色
  YELLOW = 8,           ///< 黄色
  OTHERCOLOR = 9,       ///< 其他颜色
} HeadwearColor;

/// \brief 头戴物类型
typedef enum {
  NOHEADWEARTYPE = 0,  ///<  无头戴物
  HAT = 1,             ///<  帽子
  BUILDER = 2,         ///<  builder
  SPORT = 3,           ///<  sport
  CYCLING = 4,         ///<  cycling
  OTHERTYPE = 5,       ///<  other
} HeadwearType;

/// \brief 单人脸头戴物信息
typedef struct {
  HeadwearColor color;   ///<  头戴物颜色
  float colorScore;      ///<  头戴物颜色可信度
  HeadwearType type;     ///<  头戴物类型
  float typeScore;       ///<  头戴物类型可信度
} SingleFaceHeadwear;

/// \brief 头戴物结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceHeadwearResult : public Result {
 public:
  /// \brief 构造函数
  FaceHeadwearResult();

  /// \brief 析构函数
  ~FaceHeadwearResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceHeadwears_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceHeadwears_.resize(n); }

 public:
  std::vector<SingleFaceHeadwear> faceHeadwears_;  ///< 结果容器
};

/// \brief 头戴物算法类
class ARCTERN_EXPORT_API FaceHeadwear : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceHeadwear();

  /// \brief 析构函数
  ~FaceHeadwear() override;

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
#endif  // SRC_ALGORITHMS_FACEHEADWEAR_FACEHEADWEAR_H_
