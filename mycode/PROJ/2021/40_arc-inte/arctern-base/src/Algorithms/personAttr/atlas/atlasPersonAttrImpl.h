//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONATTR_ATLASPERSONATTRIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONATTR_ATLASPERSONATTRIMPL_H_

#include "src/Algorithms/personAttr/personAttr.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasPersonAttrImpl : public ArcternAlgorithmEx {
 public:
  AtlasPersonAttrImpl();
  ~AtlasPersonAttrImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  private:
    pair<int, float> CalculateResult(const float *data, int step);
    std::vector<float> get_padded_bbox(const cv::Rect &bbox,const std::vector<float> &pad_scale);
    std::vector<cv::Rect> get_padded_rect(const int &imgWidth, const int &imgHeight,const cv::Rect &bbox, const std::vector<float> &pad_scale);
    std::array<float, 4> m_extScale;
  
};


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_PERSONATTR_ATLASPERSONATTRIMPL_H_
