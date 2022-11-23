/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceLivenessRgb.h"
#include "./faceLivenessRgbImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
using arctern::FaceLivenessRgb;
using arctern::FaceLivenessRgbRunParameter;
using arctern::FaceLivenessRgbResult;
using arctern::ErrorType;
using arctern::Result;

FaceLivenessRgbRunParameter::FaceLivenessRgbRunParameter() {
}

FaceLivenessRgbRunParameter::~FaceLivenessRgbRunParameter() noexcept {
}

FaceLivenessRgbResult::FaceLivenessRgbResult() {
}

FaceLivenessRgbResult::~FaceLivenessRgbResult() {
}

FaceLivenessRgb::FaceLivenessRgb() : impl_(nullptr) {
}

FaceLivenessRgb::~FaceLivenessRgb() noexcept {
}

ErrorType FaceLivenessRgb::Init(const InitParameter *p) {
  Release();
  impl_ = new arctern::FaceLivenessRgbImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
  return ret;
}

ErrorType FaceLivenessRgb::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceLivenessRgb::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
