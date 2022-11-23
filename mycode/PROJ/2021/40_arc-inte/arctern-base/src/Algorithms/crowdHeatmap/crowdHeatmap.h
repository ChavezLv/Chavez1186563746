/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.18
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_CROWDHEATMAP_CROWDHEATMAP_H_
#define SRC_ALGORITHMS_CROWDHEATMAP_CROWDHEATMAP_H_

#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///<

/// \brief 单张图像的人群计数信息
struct SingleCrowdHeatmap {
  std::vector<unsigned char> heatmap; ///< 人流热图
  float headCount;  ///< 人头计数
};

/// \brief 人体完整度结果返回类，存储算法的运行结果
class ARCTERN_EXPORT_API CrowdHeatmapResult : public Result {
 public:
  /// \brief 构造函数
  CrowdHeatmapResult();

  /// \brief 析构函数
  ~CrowdHeatmapResult() override;

  /// \brief 释放资源，内部调用此函数释放资源，上层可以不关注此函数
  void Release() override { crowdHeatmaps_.clear(); }

  /// \brief 调整容器的大小。上层可以不关注此函数
  /// \param n - 容器尺寸
  void resize(int n) override { crowdHeatmaps_.resize(n); }

 public:
  std::vector<SingleCrowdHeatmap> crowdHeatmaps_;  ///< 结果容器
};

/// \brief 人体完整度算法类
class ARCTERN_EXPORT_API CrowdHeatmap : public AlgorithmInterface {
 public:
  /// \brief 构造函数
  CrowdHeatmap();

  /// \brief 析构函数
  ~CrowdHeatmap() override;

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
#endif  // SRC_ALGORITHMS_CROWDHEATMAP_CROWDHEATMAP_H_
