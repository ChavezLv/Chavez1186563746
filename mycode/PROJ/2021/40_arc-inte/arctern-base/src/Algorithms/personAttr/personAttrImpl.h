/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.07
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_PERSONATTR_PERSONATTRIMPL_H_
#define SRC_ALGORITHMS_PERSONATTR_PERSONATTRIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "personAttr.h"

namespace arctern {
class PersonAttrImpl : public ArcternAlgorithm {
 public:
  PersonAttrImpl();

  ~PersonAttrImpl() override;

//  ErrorType Init(const InitParameter *p) override;
//
//  void Release() override;

  /// ErrorType Process(const RunParameter *p,Result *r) override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}

 private:
  //cv::Mat LocalGetFace(const cv::Mat &image, const RunParameter *p, int idx) override;

  ArcternRect get_person(const cv::Mat &image, const cv::Rect &bbox,
      const std::array<float, 4> ext_scale, const std::vector<float> &pad_scale);
  
  std::vector<float> get_padded_bbox(const cv::Rect &bbox,
        const std::vector<float> &pad_scale);

  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;

  std::pair<int, float> calcAttr(const float *data, size_t num_class);

 protected:

  std::array<float, 4> extScale_;
};
}  // namespace arctern
#endif  // SRC_ALGORITHMS_PERSONATTR_PERSONATTRIMPL_H_
