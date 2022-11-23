// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#pragma once
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <memory>
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API FaceTypeRunParameter : public RunParameter {
public:
  FaceTypeRunParameter();
  ~FaceTypeRunParameter();
};

class ARCTERN_EXPORT_API FaceTypeResult : public Result {
public:
  FaceTypeResult();
  ~FaceTypeResult();

  void Release() override { m_vTypeResults.clear(); }

  void resize(int n) override { m_vTypeResults.resize(n); }

public:
  std::vector<std::pair<int, float>> m_vTypeResults;
};

class ARCTERN_EXPORT_API FaceType : public AlgorithmInterface {
public:
  FaceType();
  ~FaceType();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
