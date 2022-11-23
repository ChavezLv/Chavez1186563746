// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_VEHICLEFEAT_VEHICLEFEAT_H_
#define SRC_ALGORITHMS_VEHICLEFEAT_VEHICLEFEAT_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API vehicleFeatRunParameter : public RunParameter {
public:
  vehicleFeatRunParameter();
  ~vehicleFeatRunParameter();
};

using vehicleFeatScore = std::vector<uint8>;

class ARCTERN_EXPORT_API vehicleFeatResult : public Result {
public:
  vehicleFeatResult();
  ~vehicleFeatResult();

  void Release() override { m_vFeatResults.clear(); }

  void resize(int n) override { m_vFeatResults.resize(n); }

public:
  std::vector<vehicleFeatScore> m_vFeatResults;
};

class ARCTERN_EXPORT_API vehicleFeat : public AlgorithmInterface {
public:
  vehicleFeat();
  ~vehicleFeat();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
