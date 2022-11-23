// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.07.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "personPose.h"

#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "personPoseImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personPose/trt/trtPersonPoseImpl.h"
#endif

using namespace arctern;

// class personPoseRunParameter
PersonPoseRunParameter::PersonPoseRunParameter() {}

PersonPoseRunParameter::~PersonPoseRunParameter() {}

// class personPoseResult
PersonPoseResult::PersonPoseResult() {

}

PersonPoseResult::~PersonPoseResult() {

}

// class personPose
PersonPose::PersonPose() : m_pBaseAlgImpl(nullptr) {

}

PersonPose::~PersonPose() {
  PersonPose::Release();
}

ErrorType PersonPose::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new personPoseImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new TrtPersonPoseImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#endif
  return ret;
}

void PersonPose::Release() {
  if (nullptr != m_pBaseAlgImpl) {
    m_pBaseAlgImpl->Release();
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }
}

ErrorType PersonPose::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}
