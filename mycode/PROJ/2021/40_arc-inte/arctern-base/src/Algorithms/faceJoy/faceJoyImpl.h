// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_FACEJOY_FACEJOYIMPL_H_
#define SRC_ALGORITHMS_FACEJOY_FACEJOYIMPL_H_
#include "src/Base/arcternalgorithm.h"
#include <utility>
#include <vector>

namespace arctern {
class FaceJoyImpl : public ArcternAlgorithm {
public:
  FaceJoyImpl();
  ~FaceJoyImpl();

  int GetConfigInfo(arctern::MxnetModel *model) override;

protected:
  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;

  ErrorType LocalPreProcess( const RunParameter *p, int idx);

  ErrorType CheckRunParam(const RunParameter *p) override;

  void SetLandmarks(const std::vector<float> &landmarks,std::vector<ArcternPoint> &validmarks);

private:
  std::array<int, 14> m_aLandmarksExtractIdx;
};
} // namespace arctern
#endif
