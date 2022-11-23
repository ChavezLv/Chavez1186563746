// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_VEHICLEFEAT_VEHICLEFEATIMPL_H_
#define SRC_ALGORITHMS_VEHICLEFEAT_VEHICLEFEATIMPL_H_
#include "src/Base/arcternalgorithm.h"
#include <utility>
#include <vector>

namespace arctern {
class vehicleFeatImpl : public ArcternAlgorithm {
public:
  vehicleFeatImpl();
  ~vehicleFeatImpl();

  ErrorType Init(const InitParameter *p) override;

  void Release() override;

  virtual int GetConfigInfo(arctern::MxnetModel *model) override;

protected:
  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;


  cv::Mat LocalGetFace(const cv::Mat &image,
	               const RunParameter *p, int idx);
};
} // namespace arctern
#endif
