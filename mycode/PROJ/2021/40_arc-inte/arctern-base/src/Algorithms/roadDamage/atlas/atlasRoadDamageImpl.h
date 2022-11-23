//
// Created by lzm on 2021/9/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_ROADDAMAGE_ATLASROADDAMAGEIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_ROADDAMAGE_ATLASROADDAMAGEIMPL_H_

#include "src/Algorithms/roadDamage/roadDamage.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
#include "src/Base/YoloV3.h"
#include "src/Base/yoloV5.h"
namespace arctern {
class AtlasRoadDamageImpl : public ArcternAlgorithmEx {
 public:
  AtlasRoadDamageImpl();
  ~AtlasRoadDamageImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  private:
    unique_ptr<YoloV3> m_yolo_v_3;
    unique_ptr<YoloV5> m_yolo_v_5;
    bool m_bSquareFace;
    int m_version;
    float exth_scale_face_ = 0.0;
    float extw_scale_face_ = 0.0;
};


}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_ROADDAMAGE_ATLASROADDAMAGEIMPL_H_
