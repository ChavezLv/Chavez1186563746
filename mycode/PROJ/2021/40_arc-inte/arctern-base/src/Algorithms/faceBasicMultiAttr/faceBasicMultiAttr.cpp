/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.25
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./faceBasicMultiAttr.h"

#ifdef MXNET_ALLOWED
#include "./faceBasicMultiAttrImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TVM_ALLOWED
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "tvm/tvmFaceBasicMultiAttrImpl.h"
#elif  defined(TRT_ALLOWED)
#include "trt/TrtFaceBasicMultiAttrImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/faceBasicMultiAttr/atlas/atlasFaceBasicMultiAttrImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif

using arctern::FaceBasicMultiAttr;
using arctern::FaceBasicMultiAttrRunParameter;
using arctern::FaceBasicMultiAttrResult;
using arctern::ErrorType;
using arctern::Result;

FaceBasicMultiAttrRunParameter::FaceBasicMultiAttrRunParameter() {
}

FaceBasicMultiAttrRunParameter::~FaceBasicMultiAttrRunParameter() noexcept {
}

FaceBasicMultiAttrResult::FaceBasicMultiAttrResult() {
}

FaceBasicMultiAttrResult::~FaceBasicMultiAttrResult() {
}

FaceBasicMultiAttr::FaceBasicMultiAttr() : impl_(nullptr) {
}

FaceBasicMultiAttr::~FaceBasicMultiAttr() noexcept {
  FaceBasicMultiAttr::Release();
}

ErrorType FaceBasicMultiAttr::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceBasicMultiAttrImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif  TVM_ALLOWED
  impl_ = new arctern::TvmFaceBasicMultiAttrImpl;
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new TrtFaceBasicMultiAttrImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*) impl_,p);
#elif defined(ATLAS_ALLOWED)
impl_ = new AtlasFaceBasicMultiAttrImpl;
auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*) impl_,p);
#endif

  if(ErrorType::ERR_SUCCESS != ret) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType FaceBasicMultiAttr::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceBasicMultiAttr::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
