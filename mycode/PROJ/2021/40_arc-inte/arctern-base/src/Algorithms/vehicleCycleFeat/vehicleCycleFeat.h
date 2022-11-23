// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_VEHICLECYCLEFEAT_VEHICLECYCLEFEAT_H_
#define SRC_ALGORITHMS_VEHICLECYCLEFEAT_VEHICLECYCLEFEAT_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API VehicleCycleFeatRunParameter
    : public RunParameter {
 public:
  VehicleCycleFeatRunParameter();
  ~VehicleCycleFeatRunParameter();
};

class ARCTERN_EXPORT_API VehicleCycleFeatResult : public Result {
 public:
  VehicleCycleFeatResult();
  ~VehicleCycleFeatResult();

  void Release() override { m_vFeatResults.clear(); }

  void resize(int n) override { m_vFeatResults.resize(n); }

 public:
  std::vector<ArcternFeature> m_vFeatResults;
};

class ARCTERN_EXPORT_API VehicleCycleFeat : public AlgorithmInterface {
 public:
  VehicleCycleFeat();
  ~VehicleCycleFeat();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


 private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
