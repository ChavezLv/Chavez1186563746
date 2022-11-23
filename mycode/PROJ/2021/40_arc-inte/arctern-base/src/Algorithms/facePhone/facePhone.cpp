/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "facePhone.h"
#ifdef  MXNET_ALLOWED
#include "./facePhoneImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/facePhone/trt/TrtFacePhoneImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif defined(ATLAS_ALLOWED)
#include "atlas/atlasFacePhoneImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
using arctern::FacePhone;
using arctern::FacePhoneRunParameter;
using arctern::FacePhoneResult;
using arctern::ErrorType;
using arctern::Result;

FacePhoneRunParameter::FacePhoneRunParameter() {
}

FacePhoneRunParameter::~FacePhoneRunParameter() noexcept {
}

FacePhoneResult::FacePhoneResult() {
}

FacePhoneResult::~FacePhoneResult() {
}

FacePhone::FacePhone() : impl_(nullptr) {
}

FacePhone::~FacePhone() noexcept {
  FacePhone::Release();
}

ErrorType FacePhone::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::FacePhoneImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif  defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFacePhoneImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*) impl_,p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new arctern::AtlasFacePhoneImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*) impl_,p);
#endif
  return ret;

}

ErrorType FacePhone::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}

void FacePhone::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
