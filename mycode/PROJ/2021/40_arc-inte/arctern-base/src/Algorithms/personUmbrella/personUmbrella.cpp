
#include "personUmbrella.h"

#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "personUmbrellaImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personUmbrella/trt/TrtPersonUmbrellaImpl.h"
#elif ATLAS_ALLOWED
#include "atlas/atlasPersonUmbrellaImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif

using namespace arctern;

// class PersonUmbrellaRunParameter
PersonUmbrellaRunParameter::PersonUmbrellaRunParameter() {}

PersonUmbrellaRunParameter::~PersonUmbrellaRunParameter() {}

// class PersonUmbrellaResult
PersonUmbrellaResult::PersonUmbrellaResult() : m_vClassifyResults{} {}

PersonUmbrellaResult::~PersonUmbrellaResult() {}

// class PersonUmbrella
PersonUmbrella::PersonUmbrella() : m_pBaseAlgImpl{} {}

PersonUmbrella::~PersonUmbrella() {
  PersonUmbrella::Release();
}

ErrorType PersonUmbrella::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new PersonUmbrellaImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new arctern::TrtPersonUmbrellaImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#elif ATLAS_ALLOWED
  m_pBaseAlgImpl = new arctern::AtlasPersonUmbrellaImpl;
  ErrorType ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#endif
  return ret;
}

void PersonUmbrella::Release() {
  if (nullptr != m_pBaseAlgImpl) {
    m_pBaseAlgImpl->Release();
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }
}

ErrorType PersonUmbrella::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}