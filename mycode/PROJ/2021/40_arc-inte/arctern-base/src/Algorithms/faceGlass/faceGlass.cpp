/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceGlass.h"

#ifdef MXNET_ALLOWED
#include "./src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "faceGlassImpl.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "tvm/tvmFaceGlassImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/faceGlass/trt/TrtFaceGlassImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif ATLAS_ALLOWED
#include "src/Algorithms/faceGlass/atlas/atlasFaceGlassImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
using arctern::FaceGlass;
using arctern::FaceGlassRunParameter;
using arctern::FaceGlassResult;
using arctern::ErrorType;
using arctern::Result;

FaceGlassRunParameter::FaceGlassRunParameter() {
}

FaceGlassRunParameter::~FaceGlassRunParameter() noexcept {
}

FaceGlassResult::FaceGlassResult() {
}

FaceGlassResult::~FaceGlassResult() {
}

FaceGlass::FaceGlass() : impl_(nullptr) {
}

FaceGlass::~FaceGlass() noexcept {
  FaceGlass::Release();
}

ErrorType FaceGlass::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceGlassImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceGlassImpl();
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif  defined(TRT_ALLOWED)
  impl_ = new TrtFaceGlassImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_, p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasFaceGlassImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType FaceGlass::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceGlass::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
