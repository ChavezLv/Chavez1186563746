//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEQUALITY_ATLASFACEQUALITYIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEQUALITY_ATLASFACEQUALITYIMPL_H_

#include "src/Algorithms/faceQuality/faceQuality.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasFaceQualityImpl : public ArcternAlgorithmEx {
 public:
  AtlasFaceQualityImpl();
  ~AtlasFaceQualityImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  private:

  float CalculateResult(const float *data0, const float *data1);


  int m_outputNum;
  bool m_bGrayInput;
};


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEQUALITY_ATLASFACEQUALITYIMPL_H_
