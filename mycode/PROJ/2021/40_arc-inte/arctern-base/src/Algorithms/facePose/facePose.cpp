/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "facePose.h"

#ifdef MXNET_ALLOWED
#include "./facePoseImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TVM_ALLOWED)
#include "tvm/tvmFacePoseImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/facePose/trt/TrtFacePoseImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "src/Algorithms/facePose/atlas/atlasFacePoseImpl.h"
#endif

using arctern::FacePose;
using arctern::FacePoseRunParameter;
using arctern::FacePoseResult;
using arctern::ErrorType;
using arctern::Result;

FacePoseRunParameter::FacePoseRunParameter() {
}

FacePoseRunParameter::~FacePoseRunParameter() noexcept {
}

FacePoseResult::FacePoseResult() {
}

FacePoseResult::~FacePoseResult() {
}

FacePose::FacePose() : impl_(nullptr) {
}

FacePose::~FacePose() noexcept {
  FacePose::Release();
}

ErrorType FacePose::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FacePoseImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFacePoseImpl;
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new TrtFacePoseImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new AtlasFacePoseImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType FacePose::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FacePose::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
