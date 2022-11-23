/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./faceBangs.h"
#ifdef MXNET_ALLOWED
#include "./faceBangsImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/faceBangs/trt/TrtFaceBangsImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif
using arctern::FaceBangs;
using arctern::FaceBangsRunParameter;
using arctern::FaceBangsResult;
using arctern::ErrorType;
using arctern::Result;

FaceBangsRunParameter::FaceBangsRunParameter() {
}

FaceBangsRunParameter::~FaceBangsRunParameter() noexcept {
}

FaceBangsResult::FaceBangsResult() {
}

FaceBangsResult::~FaceBangsResult() {
}

FaceBangs::FaceBangs() : impl_(nullptr) {
}

FaceBangs::~FaceBangs() noexcept {
  FaceBangs::Release();
}

ErrorType FaceBangs::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceBangsImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
  return ret;
#elif TRT_ALLOWED
  impl_ = new TrtFaceBangsImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType FaceBangs::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceBangs::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
