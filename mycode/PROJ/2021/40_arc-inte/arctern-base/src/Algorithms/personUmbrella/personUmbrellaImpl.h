// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.29
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_PERSONUMBRELLA_PERSONUMBRELLAIMPL_H_
#define SRC_ALGORITHMS_PERSONUMBRELLA_PERSONUMBRELLAIMPL_H_
#include "src/Base/arcternalgorithm.h"
#include <vector>

namespace arctern {
class PersonUmbrellaImpl : public ArcternAlgorithm {
public:
  PersonUmbrellaImpl();
  ~PersonUmbrellaImpl();
  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

protected:

  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;
  int GetConfigInfo(arctern::MxnetModel *model) { return 1; }

private:
  std::pair<int, float> calcType(const float *p_data, const size_t class_num);
  cv::Rect check_rect(const int width, const int height, const cv::Rect bbox);
};
} // namespace arctern
#endif
