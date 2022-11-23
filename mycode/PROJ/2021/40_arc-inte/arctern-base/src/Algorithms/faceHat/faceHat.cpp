/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./faceHat.h"
#ifdef MXNET_ALLOWED
#include "./faceHatImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "tvm/tvmFaceHatImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "trt/TrtFaceHatImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "atlas/atlasFaceHatImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
using arctern::FaceHat;
using arctern::FaceHatRunParameter;
using arctern::FaceHatResult;
using arctern::ErrorType;
using arctern::Result;

FaceHatRunParameter::FaceHatRunParameter() {
}

FaceHatRunParameter::~FaceHatRunParameter() noexcept {
}

FaceHatResult::FaceHatResult() {
}

FaceHatResult::~FaceHatResult() {
}

FaceHat::FaceHat() : impl_(nullptr) {
}

FaceHat::~FaceHat() noexcept {
  FaceHat::Release();
}

ErrorType FaceHat::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceHatImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceHatImpl();
  auto ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceHatImpl();
  auto ret =arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new arctern::AtlasFaceHatImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType FaceHat::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceHat::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
