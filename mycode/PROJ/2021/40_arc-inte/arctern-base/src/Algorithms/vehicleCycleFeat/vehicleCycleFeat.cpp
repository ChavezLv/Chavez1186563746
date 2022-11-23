// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "vehicleCycleFeat.h"
#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "vehicleCycleFeatImpl.h"
#elif  defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "trt/trtVehicleCycleFeatImpl.h"
#elif ATLAS_ALLOWED
#include "atlas/atlasVehicleCycleFeatImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif

using namespace arctern;

// class vehicleCycleFeatRunParameter
VehicleCycleFeatRunParameter::VehicleCycleFeatRunParameter() {

}

VehicleCycleFeatRunParameter::~VehicleCycleFeatRunParameter() {

}

// class vehicleCycleFeatResult
VehicleCycleFeatResult::VehicleCycleFeatResult() {

}

VehicleCycleFeatResult::~VehicleCycleFeatResult() {

}

// class vehicleCycleFeat
VehicleCycleFeat::VehicleCycleFeat() :m_pBaseAlgImpl(nullptr){

}

VehicleCycleFeat::~VehicleCycleFeat() {
  VehicleCycleFeat::Release();
}

ErrorType VehicleCycleFeat::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  m_pBaseAlgImpl = new vehicleCycleFeatImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **) &m_pBaseAlgImpl, p);
#elif defined(TRT_ALLOWED)
  m_pBaseAlgImpl = new TrtVehicleCycleFeatImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init(dynamic_cast<ArcternAlgorithmEx*>(m_pBaseAlgImpl),p);
#elif defined(ATLAS_ALLOWED)
  m_pBaseAlgImpl = new AtlasVehicleCycleFeatImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)m_pBaseAlgImpl,p);
#endif
  return ret;
}

void VehicleCycleFeat::Release() {
  if (nullptr != m_pBaseAlgImpl) {
    m_pBaseAlgImpl->Release();
    delete m_pBaseAlgImpl;
    m_pBaseAlgImpl = nullptr;
  }
}

ErrorType VehicleCycleFeat::Process(const RunParameter *p,Result *r) {
  if (nullptr != m_pBaseAlgImpl) {
    return m_pBaseAlgImpl->Process(p,r);
  }

  return ERR_UNEXPECT;
}

