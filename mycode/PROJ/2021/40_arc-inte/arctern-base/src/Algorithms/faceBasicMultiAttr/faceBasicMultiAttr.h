/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.25
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACEBASICMULTIATTR_FACEBASICMULTIATTR_H_
#define SRC_ALGORITHMS_FACEBASICMULTIATTR_FACEBASICMULTIATTR_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///< alg impl handle

/// \brief 人脸基础属性(5种)运行参数类
class ARCTERN_EXPORT_API FaceBasicMultiAttrRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  FaceBasicMultiAttrRunParameter();

  /// \brief 析构函数
  ~FaceBasicMultiAttrRunParameter() override ;
};

/// \brief 性别
enum class BasicGender : int {
    FAMALE = 0,  ///< 女性
    MALE   = 1,  ///< 男性
};

/// \brief 眼镜
enum class BasicGlasses : int {
    NOGLASSES  = 0,  ///< 没戴眼镜
    GLASSES    = 1,  ///< 普通眼镜
    SUNGLASSES = 2,  ///< 太阳眼镜/墨镜
};

/// \brief 口罩
enum class BasicMask : int {
    NOMASK = 0,    ///< 没戴口罩
    MALE   = 1,    ///< 戴了口罩
};

/// \brief 头戴物类型
enum class BasicHeadwear : int {
  NOHEADWEAR = 0,      ///<  无头戴物
  HAT = 1,             ///<  帽子
  SPORT = 2,           ///<  sport
  CYCLING = 3,         ///<  cycling
  BUILDER = 4,         ///<  builder
  OTHERTYPE = 5,       ///<  other
};

/// \brief 单人脸基础属性信息
typedef struct {
  BasicGender   gender;        ///<  性别
  float         genderScore;   ///<  性别可信度
  int           age;           ///<  年龄
  float         ageScore;      ///<  年龄可信度
  BasicGlasses  glasses;       ///<  眼镜类型
  float         glassesScore;  ///<  眼镜类型可信度
  BasicMask     mask;          ///<  口罩
  float         maskScore;     ///<  口罩可信度
  BasicHeadwear headwear;      ///<  头戴物类型
  float         headwearScore; ///<  头戴物类型可信度
} SingleFaceBasicMultiAttr;

/// \brief 人脸基础属性返回类，存储算法的运行结果
class ARCTERN_EXPORT_API FaceBasicMultiAttrResult : public Result {
 public:
  /// \brief 构造函数
  FaceBasicMultiAttrResult();

  /// \brief 析构函数
  ~FaceBasicMultiAttrResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { faceBasicMultiAttrs_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { faceBasicMultiAttrs_.resize(n); }

 public:
  std::vector<SingleFaceBasicMultiAttr> faceBasicMultiAttrs_;  ///< 结果容器
};

/// \brief 人脸基础属性算法类
class ARCTERN_EXPORT_API FaceBasicMultiAttr : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  FaceBasicMultiAttr();

  /// \brief 析构函数
  ~FaceBasicMultiAttr() override;

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
#endif  // SRC_ALGORITHMS_FACEBASICMULTIATTR_FACEBASICMULTIATTR_H_
