/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.04
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "personCompleteness.h"
#ifdef  MXNET_ALLOWED
#include "./personCompletenessImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personCompleteness/trt/TrtPersonCompletenessImpl.h"
#elif TVM_ALLOWED
#include "tvm/tvmPersonCompletenessImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#elif ATLAS_ALLOWED
#include "atlas/atlasPersonCompletenessImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
using arctern::PersonCompleteness;
using arctern::PersonCompletenessRunParameter;
using arctern::PersonCompletenessResult;
using arctern::ErrorType;
using arctern::Result;

PersonCompletenessRunParameter::PersonCompletenessRunParameter() {
}

PersonCompletenessRunParameter::~PersonCompletenessRunParameter() noexcept {
}

PersonCompletenessResult::PersonCompletenessResult() {
}

PersonCompletenessResult::~PersonCompletenessResult() {
}

PersonCompleteness::PersonCompleteness() : impl_(nullptr) {
}

PersonCompleteness::~PersonCompleteness() noexcept {
  PersonCompleteness::Release();
}

ErrorType PersonCompleteness::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::PersonCompletenessImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtPersonCompletenessImpl;
  auto  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif  TVM_ALLOWED
  impl_ = new arctern::TvmPersonCompletenessImpl();
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasPersonCompletenessImpl();
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*) impl_,p);
#endif

  if (ErrorType::ERR_SUCCESS != ret) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType PersonCompleteness::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void PersonCompleteness::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
