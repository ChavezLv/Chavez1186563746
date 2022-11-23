/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_VEHICLECYCLEATTR_VEHICLECYCLEATTRIMPL_H_
#define SRC_ALGORITHMS_VEHICLECYCLEATTR_VEHICLECYCLEATTRIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "vehicleCycleAttr.h"

namespace arctern {
class VehicleCycleAttrImpl : public ArcternAlgorithm {
 public:
  VehicleCycleAttrImpl();
  ~VehicleCycleAttrImpl() override;
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;};
 private:
  cv::Rect get_vehicle(const int& width, const int& height, const cv::Rect &bbox);

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  std::pair<int, float> calcAttr(const float *data, size_t num_class);
 protected:
  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_VEHICLECYCLEATTR_VEHICLECYCLEATTRIMPL_H_
