#ifndef SRC_ALGORITHMS_MANHOLECOVERDET_MANHOLECOVERDET_H_
#define SRC_ALGORITHMS_MANHOLECOVERDET_MANHOLECOVERDET_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 井盖异常检
enum ManholeCoverAbnormalType {
  MANHOLE_COVER_ABNORMAL = 0, ///< 井盖正常
  MANHOLE_COVER_NORMAL = 1,    ///< 井盖异常
};

/// \brief 井盖异常检测结果
typedef struct {
  arctern::ArcternRect rect;      ///< 物体框
  float score;  ///< 可信度
  ManholeCoverAbnormalType type;   ///< 物体类型
}MANHOLECOVER_DETECT_TINY_INFO;

using DETECT_INFOS = std::vector<MANHOLECOVER_DETECT_TINY_INFO>;

/// \brief 井盖异常检测运行参数类
class ARCTERN_EXPORT_API ManHoleCoverDetRunParameter final : public RunParameter {
 public:
  /// \brief 构造函数
  ManHoleCoverDetRunParameter();

  /// \brief 析构函数
  ~ManHoleCoverDetRunParameter() final;

  /// \brief 检查函数
  ErrorType CheckSize() const override;
};

/// \brief 井盖异常检测结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API ManHoleCoverDetResult : public Result {
 public:
  /// \brief 构造函数
  ManHoleCoverDetResult();

  /// \brief 析构函数
  ~ManHoleCoverDetResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { detInfos_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { detInfos_.resize(n); }

 public:
  std::vector<DETECT_INFOS> detInfos_;   ///<  检测信息
};

/// \brief 井盖异常检测算法类
class ARCTERN_EXPORT_API ManHoleCoverDet : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  ManHoleCoverDet();

  /// \brief 析构函数
  ~ManHoleCoverDet() override;

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
#endif  // SRC_ALGORITHMS_MANHOLECOVERDET_MANHOLECOVERDET_H_
