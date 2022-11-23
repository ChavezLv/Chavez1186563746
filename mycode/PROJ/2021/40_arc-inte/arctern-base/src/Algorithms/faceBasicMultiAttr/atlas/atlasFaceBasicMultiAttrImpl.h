//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEBASICMULTIATTR_ATLASFACEBASICMULTIATTRIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEBASICMULTIATTR_ATLASFACEBASICMULTIATTRIMPL_H_

#include "src/Algorithms/faceBasicMultiAttr/faceBasicMultiAttr.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasFaceBasicMultiAttrImpl : public ArcternAlgorithmEx {
 public:
  AtlasFaceBasicMultiAttrImpl();
  ~AtlasFaceBasicMultiAttrImpl() override ;
  
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
    std::pair<int, float> CalculateAge(const float *data, size_t num_class);
    cv::Rect GetFace(int width, int height,const cv::Rect &bbox);
    std::array<float, 4> m_extScale;
    int m_ageIndex = 1;
    int m_taskNum = 5;
    int m_ageRange = 86;
};


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEBASICMULTIATTR_ATLASFACEBASICMULTIATTRIMPL_H_
