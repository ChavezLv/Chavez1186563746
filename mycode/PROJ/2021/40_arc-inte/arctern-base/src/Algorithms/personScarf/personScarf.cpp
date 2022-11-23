
#include "personScarf.h"

#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "personScarfImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personScarf/trt/TrtPersonScarfImpl.h"
#endif

using namespace arctern;

// class PersonScarfRunParameter
PersonScarfRunParameter::PersonScarfRunParameter() {}

PersonScarfRunParameter::~PersonScarfRunParameter() {}

// class PersonScarfResult
PersonScarfResult::PersonScarfResult() : m_vClassifyResults{} {}

PersonScarfResult::~PersonScarfResult() {}

// class PersonScarf
PersonScarf::PersonScarf() : m_pBaseAlgImpl{} {}

PersonScarf::~PersonScarf() {
  PersonScarf::Release();
}

ErrorType PersonScarf::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new PersonScarfImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new arctern::TrtPersonScarfImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#endif
  return ret;
}

void PersonScarf::Release() {
  if (nullptr != m_pBaseAlgImpl) {
    m_pBaseAlgImpl->Release();
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }
}

ErrorType PersonScarf::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}