// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "faceJoy.h"
#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "faceJoyImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/faceJoy/trt/TrtFaceJoyImpl.h"

#endif


using namespace arctern;

// class FaceJoyRunParameter
FaceJoyRunParameter::FaceJoyRunParameter() {}

FaceJoyRunParameter::~FaceJoyRunParameter() {}

// class FaceJoyResult
FaceJoyResult::FaceJoyResult() : m_vJoyResults{} {}

FaceJoyResult::~FaceJoyResult() {}

// class faceJoy
FaceJoy::FaceJoy() : m_pBaseAlgImpl{} {}

FaceJoy::~FaceJoy() {
  FaceJoy::Release();
}

ErrorType FaceJoy::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new FaceJoyImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new arctern::TrtFaceJoyImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#endif
  return ret;
}

void FaceJoy::Release() {
  if (nullptr != m_pBaseAlgImpl) {
    m_pBaseAlgImpl->Release();
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }
}

ErrorType FaceJoy::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}
