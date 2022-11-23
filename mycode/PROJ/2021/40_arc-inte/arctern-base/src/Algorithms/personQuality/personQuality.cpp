/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.03
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "personQuality.h"
#ifdef MXNET_ALLOWED
#include "./personQualityImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TVM_ALLOWED
#include "tvm/tvmPersonQualityImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personQuality/trt/TrtPersonQualityImpl.h"
#endif
using arctern::PersonQuality;
using arctern::PersonQualityRunParameter;
using arctern::PersonQualityResult;
using arctern::ErrorType;
using arctern::Result;

PersonQualityRunParameter::PersonQualityRunParameter() {
}

PersonQualityRunParameter::~PersonQualityRunParameter() noexcept {
}

PersonQualityResult::PersonQualityResult() {
}

PersonQualityResult::~PersonQualityResult() {
}

PersonQuality::PersonQuality() : impl_(nullptr) {
}

PersonQuality::~PersonQuality() noexcept {
  PersonQuality::Release();
}

ErrorType PersonQuality::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::PersonQualityImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TVM_ALLOWED
  impl_ = new arctern::TvmPersonQualityImpl();
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtPersonQualityImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if (ErrorType::ERR_SUCCESS != ret) {
    delete impl_;
    impl_ = nullptr;
  }


  return ret;
}

ErrorType PersonQuality::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}


void PersonQuality::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
