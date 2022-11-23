// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "vehicleFeat.h"
#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/vehicleFeat/trt/TrtVehicleFeatImpl.h"
#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasVehicleFeatImpl.h"
#endif

#include "vehicleFeatImpl.h"

using namespace arctern;

// class vehicleFeatRunParameter
vehicleFeatRunParameter::vehicleFeatRunParameter() {}

vehicleFeatRunParameter::~vehicleFeatRunParameter() {}

// class vehicleFeatResult
vehicleFeatResult::vehicleFeatResult() : m_vFeatResults{} {}

vehicleFeatResult::~vehicleFeatResult() {}

// class vehicleFeat
vehicleFeat::vehicleFeat() : m_pBaseAlgImpl{} {}

vehicleFeat::~vehicleFeat() {
  vehicleFeat::Release();
}

ErrorType vehicleFeat::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new vehicleFeatImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **)&m_pBaseAlgImpl, p);
#elif TRT_ALLOWED
  m_pBaseAlgImpl = new arctern::TrtVehicleFeatImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)m_pBaseAlgImpl, p);
#elif ATLAS_ALLOWED
  m_pBaseAlgImpl = new arctern::AtlasVehicleFeatImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)m_pBaseAlgImpl,p);
#endif
  return ret;
}

void vehicleFeat::Release() {
  if (nullptr != m_pBaseAlgImpl) {
    m_pBaseAlgImpl->Release();
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }
}

ErrorType vehicleFeat::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}
