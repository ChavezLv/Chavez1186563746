// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_FACEIDENTIFY_FACEIDENTIFY_H_
#define SRC_ALGORITHMS_FACEIDENTIFY_FACEIDENTIFY_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <memory>
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API FaceIdentifyRunParameter : public RunParameter {
public:
  FaceIdentifyRunParameter();
  ~FaceIdentifyRunParameter();
};

class ARCTERN_EXPORT_API FaceIdentifyResult : public Result {
public:
  FaceIdentifyResult();
  ~FaceIdentifyResult();

  void Release() override { m_vTypeResults.clear(); }

  void resize(int n) override { m_vTypeResults.resize(n); }

public:
  std::vector<std::pair<int, float>> m_vTypeResults;
};

class ARCTERN_EXPORT_API FaceIdentify : public AlgorithmInterface {
public:
  FaceIdentify();
  ~FaceIdentify();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
