/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceNoiseMotionblur.h"

#ifdef MXNET_ALLOWED
#include "./faceNoiseMotionblurImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiFaceNoiseMotionblurImpl.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "./tvm/tvmFaceNoiseMotionblurImpl.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "rknn/rknnFaceNoiseMotionblurImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/faceNoiseMotionblur/trt/TrtFaceNoiseMotionImpl.h"
#elif defined(RKNN2_ALLOWED)
#include "src/Algorithms/Rknn2InterfaceCommonOperation.h"
#include "rknn2/rknn2FaceNoiseMotionblurImpl.h"
#endif

using arctern::FaceNoiseMotionblur;
using arctern::FaceNoiseMotionblurRunParameter;
using arctern::FaceNoiseMotionblurResult;
using arctern::ErrorType;
using arctern::Result;

FaceNoiseMotionblurRunParameter::FaceNoiseMotionblurRunParameter() {
}

FaceNoiseMotionblurRunParameter::~FaceNoiseMotionblurRunParameter() noexcept {
}

FaceNoiseMotionblurResult::FaceNoiseMotionblurResult() {
}

FaceNoiseMotionblurResult::~FaceNoiseMotionblurResult() {
}

FaceNoiseMotionblur::FaceNoiseMotionblur() : impl_(nullptr) {
}

FaceNoiseMotionblur::~FaceNoiseMotionblur() noexcept {
  FaceNoiseMotionblur::Release();
}

ErrorType FaceNoiseMotionblur::Init(const InitParameter *p) {
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceNoiseMotionblurImpl();
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiFaceNoiseMotionblurImpl();
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceNoiseMotionblurImpl();
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFaceNoiseMotionblurImpl();
  ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceNoiseMotionImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(RKNN2_ALLOWED)
  impl_ = new arctern::Rknn2FaceNoiseMotionblurImpl();
  ret = arctern::Rknn2InterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#endif

  return ret;
}

ErrorType FaceNoiseMotionblur::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceNoiseMotionblur::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
