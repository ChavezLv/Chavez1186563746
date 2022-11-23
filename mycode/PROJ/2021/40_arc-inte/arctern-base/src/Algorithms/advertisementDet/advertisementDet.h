

#ifndef SRC_ALGORITHMS_ADVERTISEMENTDET_ADVERTISEMENTDET_H_
#define SRC_ALGORITHMS_ADVERTISEMENTDET_ADVERTISEMENTDET_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 户外广告检测
enum AdvertisementType {
  Advertisement = 0, ///< 广告箱、广告牌
  Hang_Advertisement = 1, ///< 垂直墙体的外延式广告
};

/// \brief 户外广告检测结果
typedef struct {
  ArcternRect rect;      ///< 物体框
  float score;            ///< 可信度
  AdvertisementType type;   ///< 物体类型
}ADVERTISEMENT_DETECT_TINY_INFO;



using DETECT_INFOS = std::vector<ADVERTISEMENT_DETECT_TINY_INFO>;

/// \brief 户外广告检测运行参数类
class ARCTERN_EXPORT_API AdvertisementDetRunParameter final : public RunParameter {
 public:
  /// \brief 构造函数
  AdvertisementDetRunParameter();

  /// \brief 析构函数
  ~AdvertisementDetRunParameter() final;

  /// \brief 检查函数
  ErrorType CheckSize() const override;
};

/// \brief 户外广告检测结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API AdvertisementDetResult : public Result {
 public:
  /// \brief 构造函数
  AdvertisementDetResult();

  /// \brief 析构函数
  ~AdvertisementDetResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { detInfos_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { detInfos_.resize(n); }

 public:
  std::vector<DETECT_INFOS> detInfos_;   ///<  检测信息
};

/// \brief 户外广告检测算法类
class ARCTERN_EXPORT_API AdvertisementDet : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  AdvertisementDet();

  /// \brief 析构函数
  ~AdvertisementDet() override;

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
#endif  // SRC_ALGORITHMS_ADVERTISEMENTDET_ADVERTISEMENTDET_H_
