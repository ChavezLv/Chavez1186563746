/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_VEHICLEATTR_VEHICLEATTRIMPL_H_
#define SRC_ALGORITHMS_VEHICLEATTR_VEHICLEATTRIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "vehicleAttr.h"

namespace arctern {
class VehicleAttrImpl : public ArcternAlgorithm {
 public:
  VehicleAttrImpl();
  ~VehicleAttrImpl() override;
  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;};
  int GetModelsConfigInfo(arctern::MxnetModels *model) override;
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
 private:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;
  ErrorType CheckRunParam(const RunParameter *p) override;
  std::pair<int, float> calcAttr(const float *data, size_t num_class);
 protected:
  std::array<float, 4> extScale_;
  std::vector<std::string> carBrandNames_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_VEHICLEATTR_VEHICLEATTRIMPL_H_
