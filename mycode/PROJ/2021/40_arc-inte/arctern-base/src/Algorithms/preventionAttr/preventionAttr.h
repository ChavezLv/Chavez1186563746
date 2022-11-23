

#ifndef SRC_ALGORITHMS_PreventionAttr_PreventionAttr_H_
#define SRC_ALGORITHMS_PreventionAttr_PreventionAttr_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///< alg impl handle

/// \brief 防疫属性运行参数类
class ARCTERN_EXPORT_API PreventionAttrRunParameter : public RunParameter {
 public:
  /// \brief 构造函数
  PreventionAttrRunParameter();

  /// \brief 析构函数
  ~PreventionAttrRunParameter() override ;
};

/// \brief 
enum class ExposeNeckband : int {
    NO    = 0,  
    YES   = 1,  
    OTHER = 2,
    UNKNOWN = 3
};

/// \brief 
enum class TouchMask : int {
   NO    = 0,  
    YES   = 1,  
    OTHER = 2,
    UNKNOWN = 3
};

/// \brief 
enum class PlayPhone : int {
   NO    = 0,  
    YES   = 1,  
    OTHER = 2,
    UNKNOWN = 3
};



/// \brief 单防疫属性信息
typedef struct {
  ExposeNeckband exposeNeckband;
  float exposeNeckbandScore;
  TouchMask touchMask;
  float touchMaskScore;
  PlayPhone playPhone;
  float playPhoneScore;
} SinglePreventionAttr;

/// \brief 防疫属性返回类，存储算法的运行结果
class ARCTERN_EXPORT_API PreventionAttrResult : public Result {
 public:
  /// \brief 构造函数
  PreventionAttrResult();

  /// \brief 析构函数
  ~PreventionAttrResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { PreventionAttrs_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { PreventionAttrs_.resize(n); }

 public:
  std::vector<SinglePreventionAttr> PreventionAttrs_;  ///< 结果容器
};

/// \brief 防疫属性算法类
class ARCTERN_EXPORT_API PreventionAttr : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  PreventionAttr();

  /// \brief 析构函数
  ~PreventionAttr() override;

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
#endif  // SRC_ALGORITHMS_PreventionAttr_PreventionAttr_H_
