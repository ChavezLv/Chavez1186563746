// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.29
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "faceIdentify.h"

#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "faceIdentifyImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/faceIdentify/trt/TrtFaceIdentifyImpl.h"
#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasFaceIdentifyImpl.h"
#endif

namespace arctern {

// class FaceIdentifyRunParameter
FaceIdentifyRunParameter::FaceIdentifyRunParameter() {}
FaceIdentifyRunParameter::~FaceIdentifyRunParameter() {}

// class FaceIdentifyResult
FaceIdentifyResult::FaceIdentifyResult() : m_vTypeResults{} {}

FaceIdentifyResult::~FaceIdentifyResult() {}

// class FaceIdentify
FaceIdentify::FaceIdentify() : m_pBaseAlgImpl{} {}
FaceIdentify::~FaceIdentify() {
  FaceIdentify::Release();
}

ErrorType FaceIdentify::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new FaceIdentifyImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new arctern::TrtFaceIdentifyImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#elif ATLAS_ALLOWED
  m_pBaseAlgImpl = new arctern::AtlasFaceIdentifyImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx *)m_pBaseAlgImpl,p);
#endif

  if (ErrorType::ERR_SUCCESS != ret) {
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }

  return ret;
}
void FaceIdentify::Release() {
  delete m_pBaseAlgImpl;
  m_pBaseAlgImpl = nullptr;
}

ErrorType FaceIdentify::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}

} // namespace arctern
