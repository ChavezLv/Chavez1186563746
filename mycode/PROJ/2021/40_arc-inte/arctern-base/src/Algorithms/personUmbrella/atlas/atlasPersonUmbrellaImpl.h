//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONUMBRELLA_ATLASPERSONUMBRELLAIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONUMBRELLA_ATLASPERSONUMBRELLAIMPL_H_

#include "src/Algorithms/personUmbrella/personUmbrella.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasPersonUmbrellaImpl : public ArcternAlgorithmEx {
 public:
  AtlasPersonUmbrellaImpl();
  ~AtlasPersonUmbrellaImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  private:
    cv::Rect check_rect(const int width, const int height,const cv::Rect bbox);
    pair<int, float> CalculateResult(const float *data, int step);
    std::array<float, 4> m_extScale;
};


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONUMBRELLA_ATLASPERSONUMBRELLAIMPL_H_
