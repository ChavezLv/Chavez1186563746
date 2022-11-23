// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#ifndef SRC_ALGORITHMS_POLICESUIT_POLICESUIT_H_
#define SRC_ALGORITHMS_POLICESUIT_POLICESUIT_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <memory>
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API PoliceSuitRunParameter : public RunParameter {
public:
  PoliceSuitRunParameter();
  ~PoliceSuitRunParameter();
};

enum PoliceSuitClassify {
  INVALID_RESULT = -1, // 无效分类
  NOT_POLICE_SUIT,     // 非警服
  POLICE_SUIT          // 是警服
};

class ARCTERN_EXPORT_API PoliceSuitResult : public Result {
public:
  PoliceSuitResult();
  ~PoliceSuitResult();

  void Release() override { m_vSuitResults.clear(); }

  void resize(int n) override { m_vSuitResults.resize(n); }

public:
  std::vector<std::pair<PoliceSuitClassify, float>> m_vSuitResults;
};

class ARCTERN_EXPORT_API PoliceSuit : public AlgorithmInterface {
public:
  PoliceSuit();
  ~PoliceSuit();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
