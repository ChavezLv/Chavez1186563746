/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceImageColor.h"

#ifdef MXNET_ALLOWED
#include "./faceImageColorImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiFaceImageColorImpl.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "./tvm/tvmFaceImageColorImpl.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "./rknn/rknnFaceImageColorImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/faceImageColor/trt/TrtFaceImageColorImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif

using arctern::FaceImageColor;
using arctern::FaceImageColorRunParameter;
using arctern::FaceImageColorResult;
using arctern::ErrorType;
using arctern::Result;

FaceImageColorRunParameter::FaceImageColorRunParameter() {
}

FaceImageColorRunParameter::~FaceImageColorRunParameter() noexcept {
}

FaceImageColorResult::FaceImageColorResult() {
}

FaceImageColorResult::~FaceImageColorResult() {
}

FaceImageColor::FaceImageColor() : impl_(nullptr) {
}

FaceImageColor::~FaceImageColor() noexcept {
  FaceImageColor::Release();
}

ErrorType FaceImageColor::Init(const InitParameter *p) {
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceImageColorImpl();
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiFaceImageColorImpl;
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceImageColorImpl;
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFaceImageColorImpl;
  ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceImageColorImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  return ret;
}

ErrorType FaceImageColor::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceImageColor::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
