/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./vehicleAttr.h"
#ifdef MXNET_ALLOWED
#include "./vehicleAttrImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "trt/TrtVehicleAttrImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif ATLAS_ALLOWED
#include "atlas/atlasVehicleAttrImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif

using arctern::VehicleAttr;
using arctern::VehicleAttrRunParameter;
using arctern::VehicleAttrResult;
using arctern::ErrorType;
using arctern::Result;

VehicleAttrRunParameter::VehicleAttrRunParameter() {
}

VehicleAttrRunParameter::~VehicleAttrRunParameter() noexcept {
}

VehicleAttrResult::VehicleAttrResult() {
}

VehicleAttrResult::~VehicleAttrResult() {
}

VehicleAttr::VehicleAttr() : impl_(nullptr) {
}

VehicleAttr::~VehicleAttr() noexcept {
}

ErrorType VehicleAttr::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::VehicleAttrImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtVehicleAttrImpl();
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasVehicleAttrImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType VehicleAttr::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void VehicleAttr::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
