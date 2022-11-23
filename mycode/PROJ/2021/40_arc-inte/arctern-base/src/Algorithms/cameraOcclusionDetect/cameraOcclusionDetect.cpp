/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./cameraOcclusionDetect.h"
#include "./cameraOcclusionDetectImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
using arctern::CameraOcclusionDetect;
using arctern::CameraOcclusionDetectRunParameter;
using arctern::CameraOcclusionDetectResult;
using arctern::ErrorType;
using arctern::Result;

class CameraOcclusionDetect::Impl {
 public:
  friend CameraOcclusionDetect;
  Impl() {}
  std::shared_ptr<CameraOcclusionDetectImpl> occDetImpl_ = nullptr;
};

CameraOcclusionDetectRunParameter::CameraOcclusionDetectRunParameter() {
}

CameraOcclusionDetectRunParameter::~CameraOcclusionDetectRunParameter() noexcept {
}

CameraOcclusionDetectResult::CameraOcclusionDetectResult() {
}

CameraOcclusionDetectResult::~CameraOcclusionDetectResult() {
}

CameraOcclusionDetect::CameraOcclusionDetect() : impl_(nullptr) {
}

CameraOcclusionDetect::~CameraOcclusionDetect() noexcept {
}

ErrorType CameraOcclusionDetect::Init(CameraOcclusionDetectInitParameter *p) {
  Release();
  impl_ = std::make_shared<Impl>();
  impl_->occDetImpl_ = std::make_shared<arctern::CameraOcclusionDetectImpl>();
  auto ret = impl_->occDetImpl_->Init(p);
  if (ret != ErrorType::ERR_SUCCESS) return ret;
  return ErrorType::ERR_SUCCESS;
}

const Result *CameraOcclusionDetect::GetRealBackground(const RunParameter *p) {
  if ( impl_ == nullptr || nullptr == impl_->occDetImpl_) {
    return nullptr;
  }
  return impl_->occDetImpl_->GetRealBackground(p);
}

ErrorType CameraOcclusionDetect::Process(const RunParameter *p,Result *r) {
  if ( impl_ == nullptr || nullptr == impl_->occDetImpl_) {
    return ERR_UNEXPECT;
  }
  return impl_->occDetImpl_->Process(p,r);
}

void CameraOcclusionDetect::Release() {
  if ( impl_ == nullptr || nullptr == impl_->occDetImpl_) {
    return;
  }
  impl_->occDetImpl_->Release();
}
