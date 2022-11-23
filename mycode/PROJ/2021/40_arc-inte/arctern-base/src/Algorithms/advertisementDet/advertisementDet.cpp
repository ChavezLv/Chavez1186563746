/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "advertisementDet.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasAdvertisementDetImpl.h"
#endif

using namespace arctern;

AdvertisementDetRunParameter::AdvertisementDetRunParameter() {

}

AdvertisementDetRunParameter::~AdvertisementDetRunParameter() {

}

ErrorType AdvertisementDetRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

AdvertisementDetResult::AdvertisementDetResult() {
}

AdvertisementDetResult::~AdvertisementDetResult() noexcept {
}

AdvertisementDet::AdvertisementDet() : impl_(nullptr) {
}

AdvertisementDet::~AdvertisementDet() noexcept {
  AdvertisementDet::Release();
}

ErrorType AdvertisementDet::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasAdvertisementDetImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void AdvertisementDet::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType AdvertisementDet::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto rParam = dynamic_cast<const AdvertisementDetRunParameter*>(p);
    assert(rParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


