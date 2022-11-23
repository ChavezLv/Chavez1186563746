//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONHEADWEAR_ATLASPERSONHEADWEARIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONHEADWEAR_ATLASPERSONHEADWEARIMPL_H_

#include "src/Algorithms/personHeadwear/personHeadwear.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasPersonHeadwearImpl : public ArcternAlgorithmEx {
 public:
  AtlasPersonHeadwearImpl();
  ~AtlasPersonHeadwearImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  private:
    
    std::array<float, 4> m_extScale;
};


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONHEADWEAR_ATLASPERSONHEADWEARIMPL_H_
