/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceQualityPose.h"

#ifdef MXNET_ALLOWED
#include "./faceQualityPoseImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TVM_ALLOWED)
#include "tvm/tvmFaceQualityPoseImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#endif
using arctern::FaceQualityPose;
using arctern::FaceQualityPoseRunParameter;
using arctern::FaceQualityPoseResult;
using arctern::ErrorType;
using arctern::Result;

FaceQualityPoseRunParameter::FaceQualityPoseRunParameter() {
}

FaceQualityPoseRunParameter::~FaceQualityPoseRunParameter() noexcept {
}

FaceQualityPoseResult::FaceQualityPoseResult() {
}

FaceQualityPoseResult::~FaceQualityPoseResult() {
}

FaceQualityPose::FaceQualityPose() : impl_(nullptr) {
}

FaceQualityPose::~FaceQualityPose() noexcept {
}

ErrorType FaceQualityPose::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceQualityPoseImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceQualityPoseImpl;
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#endif

  if(ErrorType::ERR_SUCCESS != ret) {
    delete impl_;
    impl_ = nullptr;
  }
  return ret;
}

ErrorType FaceQualityPose::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceQualityPose::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
