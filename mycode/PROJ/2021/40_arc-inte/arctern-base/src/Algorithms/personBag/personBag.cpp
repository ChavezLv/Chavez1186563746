// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "personBag.h"

#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "personBagImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personBag/trt/TrtPersonBagImpl.h"
#endif

using namespace arctern;

// class personBagRunParameter
PersonBagRunParameter::PersonBagRunParameter() {}

PersonBagRunParameter::~PersonBagRunParameter() {}

// class personBagResult
PersonBagResult::PersonBagResult() : m_vClassifyResults{} {}

PersonBagResult::~PersonBagResult() {}

// class personBag
PersonBag::PersonBag() : m_pBaseAlgImpl{} {}

PersonBag::~PersonBag() {
  PersonBag::Release();
}

ErrorType PersonBag::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new personBagImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new arctern::TrtPersonBagImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#endif
  return ret;
}

void PersonBag::Release() {
  if (nullptr != m_pBaseAlgImpl) {
    m_pBaseAlgImpl->Release();
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }
}

ErrorType PersonBag::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}