//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACELANDMARK_ATLASFACELANDMARKIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACELANDMARK_ATLASFACELANDMARKIMPL_H_

#include "src/Algorithms/facialLandmark/facialLandmark.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasFacialLandmarkImpl : public ArcternAlgorithmEx {
 public:
  AtlasFacialLandmarkImpl();
  ~AtlasFacialLandmarkImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  virtual ErrorType Process(const RunParameter *p,Result *r) override;

  private:
    pair<int, float> CalculateResult(const float *data, int step);
    int m_landmarkNum;
    std::vector<cv::Rect> m_vFaceRects;
};


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACELANDMARK_ATLASFACELANDMARKIMPL_H_
