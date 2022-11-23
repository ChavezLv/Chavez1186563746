/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.18
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceOcclusion.h"
#ifdef MXNET_ALLOWED
#include "./faceOcclusionImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "tvm/tvmFaceOcclusionImpl.h"
#endif
using arctern::FaceOcclusion;
using arctern::FaceOcclusionRunParameter;
using arctern::FaceOcclusionResult;
using arctern::ErrorType;
using arctern::Result;

FaceOcclusionRunParameter::FaceOcclusionRunParameter() {
}

FaceOcclusionRunParameter::~FaceOcclusionRunParameter() noexcept {
}

FaceOcclusionResult::FaceOcclusionResult() {
}

FaceOcclusionResult::~FaceOcclusionResult() {
}

FaceOcclusion::FaceOcclusion() : impl_(nullptr) {
}

FaceOcclusion::~FaceOcclusion() noexcept {
}

ErrorType FaceOcclusion::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceOcclusionImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceOcclusionImpl;
  auto ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType FaceOcclusion::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}
void FaceOcclusion::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
