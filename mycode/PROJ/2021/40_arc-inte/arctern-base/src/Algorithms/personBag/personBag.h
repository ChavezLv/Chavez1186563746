// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_PERSONBAG_PERSONBAG_H_
#define SRC_ALGORITHMS_PERSONBAG_PERSONBAG_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API PersonBagRunParameter : public RunParameter {
public:
  PersonBagRunParameter();
  ~PersonBagRunParameter();
};

using personBagClassify = std::pair<int, float>;

class ARCTERN_EXPORT_API PersonBagResult : public Result {
public:
  PersonBagResult();
  ~PersonBagResult();

  void Release() override { m_vClassifyResults.clear(); }

  void resize(int n) override { m_vClassifyResults.resize(n); }

public:
  std::vector<personBagClassify> m_vClassifyResults;
};

class ARCTERN_EXPORT_API PersonBag : public AlgorithmInterface {
public:
  PersonBag();
  ~PersonBag();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
