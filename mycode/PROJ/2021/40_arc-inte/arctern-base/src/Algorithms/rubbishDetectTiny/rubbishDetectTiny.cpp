/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "rubbishDetectTiny.h"
#include "../src/log/logger.h"


#ifdef MXNET_ALLOWED
#include "./rubbishDetectTinyImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"

#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasRubbishDetectTinyImpl.h"

#elif defined (TRT_ALLOWED)
#include "src/Algorithms/rubbishDetectTiny/trt/TrtRubbishDetectTinyImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"

#endif

using namespace arctern;

RubbishDetectTinyRunParameter::RubbishDetectTinyRunParameter() {

}

RubbishDetectTinyRunParameter::~RubbishDetectTinyRunParameter() {

}

ErrorType RubbishDetectTinyRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

RubbishDetectTinyResult::RubbishDetectTinyResult() {
}

RubbishDetectTinyResult::~RubbishDetectTinyResult() noexcept {
}

RubbishDetectTiny::RubbishDetectTiny() : impl_(nullptr) {
}

RubbishDetectTiny::~RubbishDetectTiny() noexcept {
  RubbishDetectTiny::Release();
}

ErrorType RubbishDetectTiny::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::RubbishDetectTinyImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasRubbishDetectTinyImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtRubbishDetectTinyImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_, p);
#endif

  assert(ret != ErrorType::ERR_SUCCESS);
  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void RubbishDetectTiny::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType RubbishDetectTiny::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto rParam = dynamic_cast<const RubbishDetectTinyRunParameter*>(p);
    assert(rParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


