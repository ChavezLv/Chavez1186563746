/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "riverFloat.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasRiverFloatImpl.h"
#endif

using namespace arctern;

RiverFloatRunParameter::RiverFloatRunParameter() {

}

RiverFloatRunParameter::~RiverFloatRunParameter() {

}

ErrorType RiverFloatRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

RiverFloatResult::RiverFloatResult() {
}

RiverFloatResult::~RiverFloatResult() noexcept {
}

RiverFloat::RiverFloat() : impl_(nullptr) {
}

RiverFloat::~RiverFloat() noexcept {
  RiverFloat::Release();
}

ErrorType RiverFloat::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasRiverFloatImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void RiverFloat::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType RiverFloat::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const RiverFloatRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


