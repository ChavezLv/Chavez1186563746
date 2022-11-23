/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hangStreetDet.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasHangStreetDetImpl.h"
#endif

using namespace arctern;

HangStreetDetRunParameter::HangStreetDetRunParameter() {

}

HangStreetDetRunParameter::~HangStreetDetRunParameter() {

}

ErrorType HangStreetDetRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

HangStreetDetResult::HangStreetDetResult() {
}

HangStreetDetResult::~HangStreetDetResult() noexcept {
}

HangStreetDet::HangStreetDet() : impl_(nullptr) {
}

HangStreetDet::~HangStreetDet() noexcept {
  HangStreetDet::Release();
}

ErrorType HangStreetDet::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasHangStreetDetImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void HangStreetDet::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType HangStreetDet::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto rParam = dynamic_cast<const HangStreetDetRunParameter*>(p);
    assert(rParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


