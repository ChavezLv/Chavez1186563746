//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEPOSE_ATLASFACEPOSEIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEPOSE_ATLASFACEPOSEIMPL_H_

#include "src/Algorithms/facePose/facePose.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasFacePoseImpl : public ArcternAlgorithmEx {
 public:
  AtlasFacePoseImpl();
  ~AtlasFacePoseImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  private:
  FacePoseLevel CalculateResult(const float *data, int step);
  cv::Rect GetFace(int width, int height, const cv::Rect &bbox);
};


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_FACEPOSE_ATLASFACEPOSEIMPL_H_
