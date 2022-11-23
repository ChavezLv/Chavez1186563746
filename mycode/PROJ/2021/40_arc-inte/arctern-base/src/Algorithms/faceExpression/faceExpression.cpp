/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./faceExpression.h"

#ifdef MXNET_ALLOWED
#include "./faceExpressionImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "tvm/tvmFaceExpressionImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "trt/TrtFaceExpressionImpl.h"
#endif
using arctern::FaceExpression;
using arctern::FaceExpressionRunParameter;
using arctern::FaceExpressionResult;
using arctern::ErrorType;
using arctern::Result;

FaceExpressionRunParameter::FaceExpressionRunParameter() {
}

FaceExpressionRunParameter::~FaceExpressionRunParameter() noexcept {
}

FaceExpressionResult::FaceExpressionResult() {
}

FaceExpressionResult::~FaceExpressionResult() {
}

FaceExpression::FaceExpression() : impl_(nullptr) {
}

FaceExpression::~FaceExpression() noexcept {
  FaceExpression::Release();
}

ErrorType FaceExpression::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceExpressionImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceExpressionImpl;
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceExpressionImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType FaceExpression::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceExpression::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
