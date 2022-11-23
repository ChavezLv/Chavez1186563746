
#ifndef SRC_ALGORITHMS_PERSONSCARF_PERSONSCARF_H_
#define SRC_ALGORITHMS_PERSONSCARF_PERSONSCARF_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API PersonScarfRunParameter : public RunParameter {
public:
  PersonScarfRunParameter();
  ~PersonScarfRunParameter();
};

using PersonScarfClassify = std::pair<int, float>;

class ARCTERN_EXPORT_API PersonScarfResult : public Result {
public:
  PersonScarfResult();
  ~PersonScarfResult();

  void Release() override { m_vClassifyResults.clear(); }

  void resize(int n) override { m_vClassifyResults.resize(n); }

public:
  std::vector<PersonScarfClassify> m_vClassifyResults;
};

class ARCTERN_EXPORT_API PersonScarf : public AlgorithmInterface {
public:
  PersonScarf();
  ~PersonScarf();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
