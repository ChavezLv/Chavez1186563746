/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "personFeat.h"

#ifdef MXNET_ALLOWED
#include "./personFeatImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TVM_ALLOWED
#include "tvm/tvmPersonFeatImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "trt/TrtPersonFeatImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif ATLAS_ALLOWED
#include "atlas/atlasPersonFeatImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
using arctern::PersonFeat;
using arctern::PersonFeatRunParameter;
using arctern::PersonFeatResult;
using arctern::ErrorType;
using arctern::Result;

PersonFeatRunParameter::PersonFeatRunParameter() {
}

PersonFeatRunParameter::~PersonFeatRunParameter() noexcept {
}

PersonFeatResult::PersonFeatResult() {
}

PersonFeatResult::~PersonFeatResult() {
}

PersonFeat::PersonFeat() : impl_(nullptr) {
}

PersonFeat::~PersonFeat() noexcept {
  PersonFeat::Release();
}

ErrorType PersonFeat::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new arctern::PersonFeatImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm * *) & impl_, p);
#elif  TVM_ALLOWED
  impl_ = new arctern::TvmPersonFeatImpl();
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtPersonFeatImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init(static_cast<ArcternAlgorithmEx *>(impl_), p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasPersonFeatImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if (ErrorType::ERR_SUCCESS != ret) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType PersonFeat::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void PersonFeat::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
