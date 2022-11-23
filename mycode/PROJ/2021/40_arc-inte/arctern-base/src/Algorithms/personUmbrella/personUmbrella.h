
#ifndef SRC_ALGORITHMS_PERSONUMBRELLA_PERSONBAG_H_
#define SRC_ALGORITHMS_PERSONUMBRELLA_PERSONBAG_H_
#include "include/algorithminterface.h"
#include "include/arctern.h"
#include <vector>

namespace arctern {
class AlgorithmBase;

class ARCTERN_EXPORT_API PersonUmbrellaRunParameter : public RunParameter {
public:
  PersonUmbrellaRunParameter();
  ~PersonUmbrellaRunParameter();
};

using PersonUmbrellaClassify = std::pair<int, float>;

class ARCTERN_EXPORT_API PersonUmbrellaResult : public Result {
public:
  PersonUmbrellaResult();
  ~PersonUmbrellaResult();

  void Release() override { m_vClassifyResults.clear(); }

  void resize(int n) override { m_vClassifyResults.resize(n); }

public:
  std::vector<PersonUmbrellaClassify> m_vClassifyResults;
};

class ARCTERN_EXPORT_API PersonUmbrella : public AlgorithmInterface {
public:
  PersonUmbrella();
  ~PersonUmbrella();

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


private:
  AlgorithmBase *m_pBaseAlgImpl;
};
} // namespace arctern
#endif
