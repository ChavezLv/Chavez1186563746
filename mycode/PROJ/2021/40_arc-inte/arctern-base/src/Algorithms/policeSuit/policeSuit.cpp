// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "policeSuit.h"
#ifdef MXNET_ALLOWED
#include "policeSuitImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/policeSuit/trt/TrtPoliceSuitImpl.h"
#include "src/Base/trt/ArcternAlgorithmEx.h"
#endif

namespace arctern {

// class PoliceSuitRunParameter
PoliceSuitRunParameter::PoliceSuitRunParameter() {}
PoliceSuitRunParameter::~PoliceSuitRunParameter() {}

// class PoliceSuitResult
PoliceSuitResult::PoliceSuitResult() : m_vSuitResults{} {}

PoliceSuitResult::~PoliceSuitResult() {}

// class PoliceSuit
PoliceSuit::PoliceSuit() : m_pBaseAlgImpl{} {}
PoliceSuit::~PoliceSuit() {
  PoliceSuit::Release();
}

ErrorType PoliceSuit::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new PoliceSuitImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new arctern::TrtPoliceSuitImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#endif

  if (ErrorType::ERR_SUCCESS != ret) {
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }

  return ret;
}
void PoliceSuit::Release() {
  delete m_pBaseAlgImpl;
  m_pBaseAlgImpl = nullptr;
}

ErrorType PoliceSuit::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}

} // namespace arctern
