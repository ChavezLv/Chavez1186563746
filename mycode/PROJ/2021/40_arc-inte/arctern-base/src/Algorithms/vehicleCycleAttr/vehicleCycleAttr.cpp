/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "vehicleCycleAttr.h"
#ifdef MXNET_ALLOWED
#include "./vehicleCycleAttrImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "trt/TrtVehicleCycleAttrImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif ATLAS_ALLOWED
#include "atlas/atlasVehicleCycleAttrImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif

using arctern::VehicleCycleAttr;
using arctern::VehicleCycleAttrRunParameter;
using arctern::VehicleCycleAttrResult;
using arctern::ErrorType;
using arctern::Result;

VehicleCycleAttrRunParameter::VehicleCycleAttrRunParameter() {
}

VehicleCycleAttrRunParameter::~VehicleCycleAttrRunParameter() noexcept {
}

VehicleCycleAttrResult::VehicleCycleAttrResult() {
}

VehicleCycleAttrResult::~VehicleCycleAttrResult() {
}

VehicleCycleAttr::VehicleCycleAttr() : impl_(nullptr) {
}

VehicleCycleAttr::~VehicleCycleAttr() noexcept {
}

ErrorType VehicleCycleAttr::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::VehicleCycleAttrImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtVehicleCycleAttrImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasVehicleCycleAttrImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType VehicleCycleAttr::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void VehicleCycleAttr::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
