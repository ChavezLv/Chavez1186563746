/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "roadDamage.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasRoadDamageImpl.h"
#endif

using namespace arctern;

RoadDamageRunParameter::RoadDamageRunParameter() {

}

RoadDamageRunParameter::~RoadDamageRunParameter() {

}

ErrorType RoadDamageRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

RoadDamageResult::RoadDamageResult() {
}

RoadDamageResult::~RoadDamageResult() noexcept {
}

RoadDamage::RoadDamage() : impl_(nullptr) {
}

RoadDamage::~RoadDamage() noexcept {
  RoadDamage::Release();
}

ErrorType RoadDamage::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasRoadDamageImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void RoadDamage::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType RoadDamage::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const RoadDamageRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


