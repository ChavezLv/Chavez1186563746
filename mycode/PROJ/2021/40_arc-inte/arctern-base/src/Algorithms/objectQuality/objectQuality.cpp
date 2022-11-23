/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.16
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "objectQuality.h"
#include "./objectQualityImpl.h"
#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/objectQuality/trt/TrtObjectQualityImpl.h"
#include "src/Base/trt/ArcternAlgorithmEx.h"
#endif

using arctern::ObjectQuality;
using arctern::ObjectQualityRunParameter;
using arctern::ObjectQualityResult;
using arctern::ErrorType;
using arctern::Result;

ObjectQualityRunParameter::ObjectQualityRunParameter() {
}

ObjectQualityRunParameter::~ObjectQualityRunParameter() noexcept {
}

ObjectQualityResult::ObjectQualityResult() {
}

ObjectQualityResult::~ObjectQualityResult() {
}

ObjectQuality::ObjectQuality() : impl_(nullptr) {
}

ObjectQuality::~ObjectQuality() noexcept {
  ObjectQuality::Release();
}

ErrorType ObjectQuality::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::ObjectQualityImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtObjectQualityImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(
      (ArcternAlgorithmEx *)impl_, p);
#endif
  return ret;
}


ErrorType ObjectQuality::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}


void ObjectQuality::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
