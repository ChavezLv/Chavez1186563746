// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_FACEJOY_FACEJOY_H_
#define SRC_ALGORITHMS_FACEJOY_FACEJOY_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {
class AlgorithmBase;

using FaceJoyLandmarks = std::vector<float>;

class ARCTERN_EXPORT_API FaceJoyRunParameter : public RunParameter {
public:
  FaceJoyRunParameter();
  ~FaceJoyRunParameter();

public:
  std::vector<FaceJoyLandmarks> faceLandmarksV_;
};

using faceJoyScores = std::vector<std::pair<int, float>>;

class ARCTERN_EXPORT_API FaceJoyResult : public Result {
public:
  FaceJoyResult();
  ~FaceJoyResult();

  void Release() override { m_vJoyResults.clear(); }

  void resize(int n) override { m_vJoyResults.resize(n); }

public:
  std::vector<faceJoyScores> m_vJoyResults;
};

class ARCTERN_EXPORT_API FaceJoy : public AlgorithmInterface {
public:
  FaceJoy();
  ~FaceJoy();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
