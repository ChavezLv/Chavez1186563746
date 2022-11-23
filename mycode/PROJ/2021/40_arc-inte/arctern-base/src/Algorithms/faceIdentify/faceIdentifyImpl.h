// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.29
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#pragma once
#include "src/Base/arcternalgorithm.h"
#include <vector>

namespace arctern {
class FaceIdentifyImpl : public ArcternAlgorithm {
public:
  FaceIdentifyImpl();
  ~FaceIdentifyImpl();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;

protected:
  cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter *p,
                          int idx) override;

  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;

  int GetConfigInfo(arctern::MxnetModel *model) { return 1; }

private:
  cv::Rect getFace(const int width, const int height, const cv::Rect bbox);
  std::pair<int, float> calcType(const float *p_data, const size_t class_num);

private:
  std::array<float, 4> m_aExtScale;
};
} // namespace arctern
