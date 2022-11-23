/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.07
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_PERSONATTR_PERSONATTR_H_
#define SRC_ALGORITHMS_PERSONATTR_PERSONATTR_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

using PersonTruncation = std::vector<float>;

/// \brief 人体属性运行参数类
class ARCTERN_EXPORT_API PersonAttrRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  PersonAttrRunParameter();

  /// \brief 析构函数
  ~PersonAttrRunParameter();

 public:
  std::vector<PersonTruncation> personTruncationV_;
};

/// \brief 年龄
enum class PersonAgeType : int {
  ADULT  = 0,  ///< 成年人
  CHILD  = 1,  ///< 小孩
  OLD    = 2,  ///< 老人
};

/// \brief 性别
enum class PersonGenderType : int {
  FAMALE = 0,  ///< 女性
  MALE   = 1,  ///< 男性
};

/// \brief 眼镜
enum class PersonGlassesType : int {
  NOGLASSES  = 0,  ///< 没戴眼镜
  GLASSES    = 1,  ///< 普通眼镜
  SUNGLASSES = 2,  ///< 太阳眼镜/墨镜
  UNKNOWN  = 3
};

/// \brief 头发颜色
enum class PersonHairColorType : int {
  BLACK    = 0,  ///< 黑色
  BROWN    = 1,  ///< 棕色
  GRAY     = 2,  ///< 灰色
  WHITE    = 3,  ///< 白色
  YELLOW   = 4,  ///< 黄色
  OTHERS   = 5,
  UNKNOWN  = 6
};

/// \brief 头发长短
enum class PersonHairLengthType : int {
  BALD    = 0,  ///< 秃头
  LONG    = 1,  ///< 长发
  SHORT   = 2,  ///< 短发
  UNKNOWN  = 3
};
#ifndef PERSONHEADWEARTYPE
#define PERSONHEADWEARTYPE
/// \brief 头戴物类型
enum class PersonHeadwearType : int {
  NONE      = 0,   ///<  无头戴物
  HAT       = 1,   ///<  帽子
  HELMET    = 2,   ///<  头盔
  OTHERTYPE = 3,   ///<  other
};
#endif
/// \brief 穿着颜色（上身or下身）
enum class PersonClothesColorType : int {
  BLACK    = 0,  ///< 黑色
  BLUE     = 1,  ///< 蓝色
  GRAY     = 2,  ///< 灰色
  GREEN    = 3,  ///< 绿色
  ORANGE   = 4,  ///< 橙色
  OTHERS   = 5,  ///< 其他颜色
  PURPLE   = 6,  ///< 紫色
  RED      = 7,  ///< 红色
  WHITE    = 8,  ///< 白色
  YELLOW   = 9,  ///< 黄色
  PINK     = 10,  ///< 粉色
  BROWN    = 11,  ///< 棕色
  UNKNOWN  =12    ///< 未知
};

/// \brief 衣服纹理类型（上身or下身）
enum class PersonClothesTextureType : int {
  GRID     = 0,  ///< 格子
  JOINT    = 1,  ///< 拼合
  OTHERS   = 2,  ///< 其他
  PATTERN  = 3,  ///< 图案
  PURE     = 4,  ///< 纯色
  STRIPE   = 5,  ///< 条纹
  UNIFORM  = 6,  ///< 制服
  UNKNOWN  = 7   ///< 未知
};

/// \brief 上身穿着类型
enum class PersonUpperClothesType : int {
  LONGSLEEVE    = 0,  ///< 长袖
  SHORTSLEEVE   = 1,  ///< 短袖
};

/// \brief 下身穿着类型
enum class PersonLowerClothesType : int {
  DRESS    = 0,  ///< 裙子
  PANTS    = 1,  ///< 裤子
  SHORTS   = 2,  ///< 短裤
  UNKNOWN  = 3,  ///< 未知
};

/// \brief 身体朝向
enum class PersonOrientationType : int {
  BACK    = 0,  ///< 后面
  FRONT   = 1,  ///< 前面
  SIDE    = 2,  ///< 侧面
};

/// \brief 人的类型
enum class PersonType : int {
  OTHERS     = 0,  ///< 其他
  PEDESTRIAN = 1,  ///< 行人
  RIDER      = 2,  ///< 骑行者
};

/// \brief 质量
enum class Quality : int{
  BAD = 0,       ///> 坏
  GOOD = 1      ///> 好
};

/// \brief 口罩
enum class Mask : int{
  YES = 0,       ///> 有戴口罩
  NO = 1,      ///> 没有口罩
  UNKNOWN = 2  ///> 未知
};

/// \brief 外套
enum class Overcoat : int{
  YES = 0,       ///> 有
  NO = 1,        ///> 没有
  UNKNOWN = 2    ///> 未知
};

/// \brief 单肩包
enum class SingleBag : int{
  YES = 0,       ///> 有
  NO = 1,         ///> 没有
  UNKNOWN =2
};

/// \brief 单人属性
typedef struct {
  PersonAgeType             age;          ///< 年龄 
  PersonGenderType          gender;       ///< 性别
  PersonGlassesType         glasses;      ///< 眼镜
  PersonHairColorType       hairColor;    ///< 头发颜色
  PersonHairLengthType      hairLength;   ///< 头发长短
  PersonHeadwearType        headwear;     ///< 头戴物类型
  PersonClothesColorType    upperColor;   ///< 上身穿着颜色
  PersonClothesColorType    lowerColor;   ///< 下身穿着颜色
  PersonClothesTextureType  upperTexture; ///< 上身穿着纹理类型
  PersonClothesTextureType  lowerTexture; ///< 下身穿着纹理类型
  PersonUpperClothesType    upperType;    ///< 上身穿着类型
  PersonLowerClothesType    lowerType;    ///< 下身穿着类型
  PersonOrientationType     orientation;  ///< 身体朝向
  PersonType                personType;   ///< 人的类型
  
  bool hasBackpack;     ///< 双肩包
  bool hasCart;         ///< 购物车
  bool isFrontHold;     ///< 前行
  bool hasHandbag;      ///< 手提包
  Mask hasMask;         ///< 口罩
  Overcoat hasOvercoat;     ///< 外套
  SingleBag hasSingleBag;    ///< 单肩包
  bool hasTrolleyCase;  ///< 拉杆箱
  bool usePhone;        ///< 使用电话
  Quality quality;      ///< 质量

  float  ageScore;          ///< 年龄评分
  float  genderScore;       ///< 性别评分
  float  glassesScore;      ///< 眼镜评分
  float  hairColorScore;    ///< 头发颜色评分
  float  hairLengthScore;   ///< 头发长短评分
  float  headwearScore;     ///< 头戴物类型评分
  float  upperColorScore;   ///< 上身穿着颜色评分
  float  lowerColorScore;   ///< 下身穿着颜色评分
  float  upperTextureScore; ///< 上身穿着纹理评分
  float  lowerTextureScore; ///< 下身穿着纹理评分
  float  upperTypeScore;    ///< 上身穿着类型评分
  float  lowerTypeScore;    ///< 下身穿着类型评分
  float  orientationScore;  ///< 身体朝向评分
  float  personTypeScore;   ///< 人的类型评分
  
  float hasBackpackScore;     ///< 双肩包
  float hasCartScore;         ///< 购物车
  float isFrontHoldScore;     ///< 前行
  float hasHandbagScore;      ///< 手提包
  float hasMaskScore;         ///< 口罩
  float hasOvercoatScore;     ///< 外套
  float hasSingleBagScore;    ///< 单肩包
  float hasTrolleyCaseScore;  ///< 拉杆箱
  float usePhoneScore;        ///< 使用电话
  float qualityScore;         ///< 质量得分

} SinglePersonAttr;

/// \brief 人体属性结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API PersonAttrResult : public Result {
 public:
  /// \brief 构造函数
  PersonAttrResult();

  /// \brief 析构函数
  ~PersonAttrResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { personAttrs_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { personAttrs_.resize(n); }

 public:
  std::vector<SinglePersonAttr> personAttrs_;  ///<  结果容器
};

/// \brief 人体属性算法类
class ARCTERN_EXPORT_API PersonAttr : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  PersonAttr();

  /// \brief 析构函数
  ~PersonAttr() override;

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
#endif  // SRC_ALGORITHMS_PERSONATTR_PERSONATTR_H_

