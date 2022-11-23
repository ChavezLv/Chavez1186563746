//
// Created by lzm on 2021/9/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_ROADWATER_ATLASROADWATERIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_ROADWATER_ATLASROADWATERIMPL_H_

#include "src/Algorithms/roadWater/roadWater.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
#include "src/Base/YoloV3.h"
#include "src/Base/yoloV5.h"
namespace arctern {
class AtlasRoadWaterImpl : public ArcternAlgorithmEx {
 public:
  AtlasRoadWaterImpl();
  ~AtlasRoadWaterImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

};


}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_ROADWATER_ATLASROADWATERIMPL_H_
