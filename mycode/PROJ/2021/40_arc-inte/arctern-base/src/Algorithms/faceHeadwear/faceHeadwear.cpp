/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./faceHeadwear.h"
#ifdef MXNET_ALLOWED
#include "./faceHeadwearImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/faceHeadwear/trt/TrtFaceHeadwearImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif
using arctern::FaceHeadwear;
using arctern::FaceHeadwearRunParameter;
using arctern::FaceHeadwearResult;
using arctern::ErrorType;
using arctern::Result;

FaceHeadwearRunParameter::FaceHeadwearRunParameter() {
}

FaceHeadwearRunParameter::~FaceHeadwearRunParameter() noexcept {
}

FaceHeadwearResult::FaceHeadwearResult() {
}

FaceHeadwearResult::~FaceHeadwearResult() {
}

FaceHeadwear::FaceHeadwear() : impl_(nullptr) {
}

FaceHeadwear::~FaceHeadwear() noexcept {
  FaceHeadwear::Release();
}

ErrorType FaceHeadwear::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceHeadwearImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceHeadwearImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType FaceHeadwear::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceHeadwear::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
