/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.07
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./personAttr.h"

#ifdef MXNET_ALLOWED
#include "./personAttrImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personAttr/trt/TrtPersonAttrImpl.h"
#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasPersonAttrImpl.h"
#endif
using arctern::PersonAttr;
using arctern::PersonAttrRunParameter;
using arctern::PersonAttrResult;
using arctern::ErrorType;
using arctern::Result;

PersonAttrRunParameter::PersonAttrRunParameter() {
}

PersonAttrRunParameter::~PersonAttrRunParameter() noexcept {
}

PersonAttrResult::PersonAttrResult() {
}

PersonAttrResult::~PersonAttrResult() {
}

PersonAttr::PersonAttr() : impl_(nullptr) {
}

PersonAttr::~PersonAttr() noexcept {
  PersonAttr::Release();
}

ErrorType PersonAttr::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::PersonAttrImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtPersonAttrImpl;
  ErrorType ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx *)impl_, p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasPersonAttrImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType PersonAttr::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void PersonAttr::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
