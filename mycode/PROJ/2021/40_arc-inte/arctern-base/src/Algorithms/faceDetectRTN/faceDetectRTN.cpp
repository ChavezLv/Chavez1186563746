/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:    in this interface we can only use mxnet, so delete with_mxnet
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./src/log/logger.h"
#include "./src/Algorithms/faceDetectRTN/faceDetectRTN.h"
#ifdef MXNET_ALLOWED
#include "./faceDetectRTNImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/faceDetectRTN/tvm/tvmFaceDetectRTNImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "./trt/TrtFaceDetectRTNImpl.h"
#endif
using namespace arctern;
using arctern::ErrorType;
using arctern::FaceDetectRTN;
using arctern::Result;
using arctern::FaceDetectRTNResult;

FaceDetectRTNRunParameter::FaceDetectRTNRunParameter() {

}

FaceDetectRTNRunParameter::~FaceDetectRTNRunParameter() {

}

ErrorType FaceDetectRTNRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

FaceDetectRTNResult::FaceDetectRTNResult() {
  return;
}

FaceDetectRTNResult::~FaceDetectRTNResult()  {
  return;
}

FaceDetectRTN::FaceDetectRTN(): impl_(nullptr) {
  return;
}

FaceDetectRTN::~FaceDetectRTN()  {
  FaceDetectRTN::Release();
  return;
}

ErrorType FaceDetectRTN::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceDetectRTNImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceDetectRTNImpl();
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceDetectRTNImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(dynamic_cast<ArcternAlgorithmEx*>(impl_),p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void FaceDetectRTN::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType FaceDetectRTN::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectRTNRunParameter = dynamic_cast<const FaceDetectRTNRunParameter*>(p);
    assert(faceDetectRTNRunParameter != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}
