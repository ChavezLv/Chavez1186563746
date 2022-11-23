//
// Created by lzm on 2021/9/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_MULTIDETECTCENTER_ATLASMULTIDETECTCENTERIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_MULTIDETECTCENTER_ATLASMULTIDETECTCENTERIMPL_H_
#include <map>
#include "src/Algorithms/multiDetectCenter/multiDetectCenter.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasMultiDetectCenterImpl : public ArcternAlgorithmEx {
 public:
  AtlasMultiDetectCenterImpl();
  ~AtlasMultiDetectCenterImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  protected:
  bool initMemberVariable();
  void initPostProcessParam(int img_width, int img_height, const cv::Size& osize, std::vector<int> &pad_w, std::vector<int> &pad_h, std::vector<float> &img_ratio);

  //virtual void initPreProcessor();

 public:
  bool square_rect_ = false;  // unify to output square boxes
  float exth_scale_rect_ = 0.0;
  float extw_scale_rect_ = 0.0;

 protected:
  int input_dim_w;
  int input_dim_h;
  int output_dim_w;
  int output_dim_h;
  float hm_ratio_w_;
  float hm_ratio_h_;
  bool flag_quality;
  bool flag_ind_wh;
  std::map<int, int> cls_project;
  std::vector<int> vec_cls_project;

};


}

#endif //ARCTERN_BASE_SRC_ALGORITHMS_MULTIDETECTCENTER_ATLASMULTIDETECTCENTERIMPL_H_
