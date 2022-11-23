/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceSmoke.h"
#ifdef MXNET_ALLOWED
#include "./faceSmokeImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/faceSmoke/trt/TrtFaceSmokeImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasFaceSmokeImpl.h"
#endif
using arctern::FaceSmoke;
using arctern::FaceSmokeRunParameter;
using arctern::FaceSmokeResult;
using arctern::ErrorType;
using arctern::Result;

FaceSmokeRunParameter::FaceSmokeRunParameter() {
}

FaceSmokeRunParameter::~FaceSmokeRunParameter() noexcept {
}

FaceSmokeResult::FaceSmokeResult() {
}

FaceSmokeResult::~FaceSmokeResult() {
}

FaceSmoke::FaceSmoke() : impl_(nullptr) {
}

FaceSmoke::~FaceSmoke() noexcept {
  FaceSmoke::Release();
}

ErrorType FaceSmoke::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceSmokeImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceSmokeImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init(dynamic_cast<ArcternAlgorithmEx*>(impl_),p);
  #elif defined(ATLAS_ALLOWED)
  impl_ = new arctern::AtlasFaceSmokeImpl;
  auto  ret = arctern::AtlasInterfaceCommonOperation::Init(dynamic_cast<ArcternAlgorithmEx*>(impl_),p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }
  return ret;
}

ErrorType FaceSmoke::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceSmoke::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
