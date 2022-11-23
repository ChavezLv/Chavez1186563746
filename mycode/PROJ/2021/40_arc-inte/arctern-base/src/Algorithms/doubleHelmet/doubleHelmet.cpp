/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "doubleHelmet.h"
#ifdef ATLAS_ALLOWED
#include "src/Algorithms/doubleHelmet/atlas/atlasDoubleHelmetImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
using arctern::DoubleHelmet;
using arctern::DoubleHelmetRunParameter;
using arctern::DoubleHelmetResult;
using arctern::ErrorType;
using arctern::Result;

DoubleHelmetRunParameter::DoubleHelmetRunParameter() {
}

DoubleHelmetRunParameter::~DoubleHelmetRunParameter() noexcept {
}

DoubleHelmetResult::DoubleHelmetResult() {
}

DoubleHelmetResult::~DoubleHelmetResult() {
}

DoubleHelmet::DoubleHelmet() : impl_(nullptr) {
}

DoubleHelmet::~DoubleHelmet() noexcept {
  DoubleHelmet::Release();
}

ErrorType DoubleHelmet::Init(const InitParameter *p) {
  Release();
#ifdef ATLAS_ALLOWED
  impl_ = new AtlasDoubleHelmetImpl;
  auto ret = AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType DoubleHelmet::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void DoubleHelmet::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
