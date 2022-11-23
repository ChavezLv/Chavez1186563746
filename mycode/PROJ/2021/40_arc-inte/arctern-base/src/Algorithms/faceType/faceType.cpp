// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "faceType.h"
#ifdef MXNET_ALLOWED
#include "faceTypeImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "trt/TrtFaceTypeImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif ATLAS_ALLOWED
#include "atlas/atlasFaceTypeImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
namespace arctern {

// class faceTypeRunParameter
FaceTypeRunParameter::FaceTypeRunParameter() {}
FaceTypeRunParameter::~FaceTypeRunParameter() {}

// class faceTypeResult
FaceTypeResult::FaceTypeResult() : m_vTypeResults{} {}

FaceTypeResult::~FaceTypeResult() {}

// class faceType
FaceType::FaceType() : m_pBaseAlgImpl{} {}
FaceType::~FaceType() {
  FaceType::Release();
}

ErrorType FaceType::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new faceTypeImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new TrtFaceTypeImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)m_pBaseAlgImpl,p);
#elif ATLAS_ALLOWED
  m_pBaseAlgImpl = new AtlasFaceTypeImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)m_pBaseAlgImpl,p);
#endif
  if (ErrorType::ERR_SUCCESS != ret) {
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }

  return ret;
}
void FaceType::Release() {
  delete m_pBaseAlgImpl;
  m_pBaseAlgImpl = nullptr;
}

ErrorType FaceType::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}

} // namespace arctern
