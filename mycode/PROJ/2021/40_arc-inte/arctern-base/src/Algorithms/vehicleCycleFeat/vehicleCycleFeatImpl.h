// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_VEHICLECYCLEFEAT_VEHICLECYCLEFEATIMPL_H_
#define SRC_ALGORITHMS_VEHICLECYCLEFEAT_VEHICLECYCLEFEATIMPL_H_
#include "src/Base/arcternalgorithm.h"
#include <utility>
#include <vector>

namespace arctern {
class vehicleCycleFeatImpl : public ArcternAlgorithm {
public:
  vehicleCycleFeatImpl();
  ~vehicleCycleFeatImpl();

  ErrorType Init(const InitParameter *p) override;

  void Release() override;


  int GetConfigInfo(arctern::MxnetModel *model) override;

protected:
  cv::Mat LocalPreProcess(const cv::Mat &image, const RunParameter *p,
                          int idx) override;

  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  ;
};
} // namespace arctern
#endif
