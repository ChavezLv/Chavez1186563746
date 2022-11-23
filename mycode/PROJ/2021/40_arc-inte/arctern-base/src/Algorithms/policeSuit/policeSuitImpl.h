// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#pragma once
#include "src/Base/arcternalgorithm.h"
#include <vector>
#include "policeSuit.h"

namespace arctern {
class PoliceSuitImpl : public ArcternAlgorithm {
public:
  PoliceSuitImpl();
  ~PoliceSuitImpl();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;

protected:
  cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter *p,
                          int idx) override;

  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;

  cv::Rect get_chest_roi(cv::Rect roi, int img_w, int img_h);

  int GetConfigInfo(arctern::MxnetModel *model) { return 1; }

private:
  std::pair<PoliceSuitClassify, float> calcSuit(const float *p_data,
                                                const size_t class_num);

protected:
  int m_iImageW;
  int m_iImageH;
};
} // namespace arctern
