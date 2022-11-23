//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_ATLASFACEGLUONIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_ATLASFACEGLUONIMPL_H_

#include "src/Algorithms/faceGluon/faceGluon.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
#include "src/Algorithms/faceGluon/utils/faceGluonUtil.h"
#include "src/common/featureUtil.h"
using namespace gluon;
namespace arctern {
class AtlasFaceGluonImpl : public ArcternAlgorithmEx {
 public:
  AtlasFaceGluonImpl();
  ~AtlasFaceGluonImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  private:

    string m_version;
    bool m_bNeedLandmark;
    bool m_bMaskWipe = false;
    gluon::LocalPreProcessV m_type;
    std::vector<ArcternPoint> m_lmk;
    std::array<float, 2> m_extHeightScale;
    std::array<float, 2> m_extWidthScale;
};


}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLUON_ATLASFACEGLUONIMPL_H_
