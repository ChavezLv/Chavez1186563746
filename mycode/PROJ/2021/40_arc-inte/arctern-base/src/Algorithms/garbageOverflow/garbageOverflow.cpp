/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "garbageOverflow.h"
#include "../src/log/logger.h"


#ifdef MXNET_ALLOWED
#include "./garbageOverflowImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"

#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasGarbageOverflowImpl.h"

#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/garbageOverflow/trt/TrtGarbageOverflowImpl.h"

#endif

using namespace arctern;

GarbageOverflowRunParameter::GarbageOverflowRunParameter() {

}

GarbageOverflowRunParameter::~GarbageOverflowRunParameter() {

}

ErrorType GarbageOverflowRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

GarbageOverflowResult::GarbageOverflowResult() {
}

GarbageOverflowResult::~GarbageOverflowResult() noexcept {
}

GarbageOverflow::GarbageOverflow() : impl_(nullptr) {
}

GarbageOverflow::~GarbageOverflow() noexcept {
  GarbageOverflow::Release();
}

ErrorType GarbageOverflow::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::GarbageOverflowImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasGarbageOverflowImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtGarbageOverflowImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init(\
                            (ArcternAlgorithmEx*) impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void GarbageOverflow::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType GarbageOverflow::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const GarbageOverflowRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


