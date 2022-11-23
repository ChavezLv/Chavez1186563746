/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "illegalAccumulation.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasIllegalAccumulationImpl.h"
#endif

using namespace arctern;

IllegalAccumulationRunParameter::IllegalAccumulationRunParameter() {

}

IllegalAccumulationRunParameter::~IllegalAccumulationRunParameter() {

}

ErrorType IllegalAccumulationRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

IllegalAccumulationResult::IllegalAccumulationResult() {
}

IllegalAccumulationResult::~IllegalAccumulationResult() noexcept {
}

IllegalAccumulation::IllegalAccumulation() : impl_(nullptr) {
}

IllegalAccumulation::~IllegalAccumulation() noexcept {
  IllegalAccumulation::Release();
}

ErrorType IllegalAccumulation::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasIllegalAccumulationImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void IllegalAccumulation::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType IllegalAccumulation::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto rParam = dynamic_cast<const IllegalAccumulationRunParameter*>(p);
    assert(rParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


