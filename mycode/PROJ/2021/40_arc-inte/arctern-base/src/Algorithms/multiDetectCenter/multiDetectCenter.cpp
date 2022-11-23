/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "multiDetectCenter.h"
#include "../src/log/logger.h"

#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "./centernetImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/multiDetectCenter/trt/TrtMultiDetectCenterImpl.h"
#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "src/Algorithms/multiDetectCenter/atlas/atlasMultiDetectCenterImpl.h"
#endif
using namespace arctern;
using arctern::ErrorType;
using arctern::MultiDetectCenter;
using arctern::Result;
using arctern::MultiDetectCenterResult;

MultiDetectCenterRunParameter::MultiDetectCenterRunParameter() {

}

MultiDetectCenterRunParameter::~MultiDetectCenterRunParameter() {

}

ErrorType MultiDetectCenterRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

MultiDetectCenterResult::MultiDetectCenterResult() {
}

MultiDetectCenterResult::~MultiDetectCenterResult() noexcept {
}

MultiDetectCenter::MultiDetectCenter() : impl_(nullptr) {
}

MultiDetectCenter::~MultiDetectCenter() noexcept {
  MultiDetectCenter::Release();
}

ErrorType MultiDetectCenter::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::CenterNet();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtMultiDetectCenterImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif ATLAS_ALLOWED
  impl_ = new AtlasMultiDetectCenterImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

void MultiDetectCenter::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType MultiDetectCenter::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto multiDetectRunParam = dynamic_cast<const MultiDetectCenterRunParameter*>(p);
    assert(multiDetectRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}


