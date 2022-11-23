//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEFEAT_ATLASVEHICLECYCLEFEATIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEFEAT_ATLASVEHICLECYCLEFEATIMPL_H_

#include "src/Algorithms/vehicleCycleFeat/vehicleCycleFeat.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
#include "src/common/featureUtil.h"

namespace arctern {
class AtlasVehicleCycleFeatImpl : public ArcternAlgorithmEx {
 public:
  AtlasVehicleCycleFeatImpl();
  ~AtlasVehicleCycleFeatImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;
};


}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_VEHICLECYCLEFEAT_ATLASVEHICLECYCLEFEATIMPL_H_
