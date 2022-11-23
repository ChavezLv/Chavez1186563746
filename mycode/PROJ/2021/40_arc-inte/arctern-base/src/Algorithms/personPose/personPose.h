// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.07.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_PERSONPOSE_PERSONPOS_H_
#define SRC_ALGORITHMS_PERSONPOSE_PERSONPOS_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {

class AlgorithmBase;

class ARCTERN_EXPORT_API PersonPoseRunParameter : public RunParameter {
 public:
  PersonPoseRunParameter();
  ~PersonPoseRunParameter();
};

using PersonPosePoints = std::vector<std::pair<ArcternPoint, float>>;

class ARCTERN_EXPORT_API PersonPoseResult : public Result {
 public:
  PersonPoseResult();
  ~PersonPoseResult();

  void Release() override { results_.clear(); }

  void resize(int n) override { results_.resize(n); }

 public:
  std::vector<PersonPosePoints> results_;
};

class ARCTERN_EXPORT_API PersonPose : public AlgorithmInterface {
 public:
  PersonPose();
  ~PersonPose();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


 private:
  AlgorithmBase *m_pBaseAlgImpl;
};

} // namespace arctern
#endif
