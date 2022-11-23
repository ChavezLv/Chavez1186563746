/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./licenseRecog.h"
#ifdef MXNET_ALLOWED
#include "./licenseRecogImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/licenseRecog/trt/TrtLicenseRecogImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasLicenseRecogImpl.h"
#endif
using arctern::LicenseRecog;
using arctern::LicenseRecogRunParameter;
using arctern::LicenseRecogResult;
using arctern::ErrorType;
using arctern::Result;

LicenseRecogRunParameter::LicenseRecogRunParameter() {
}

LicenseRecogRunParameter::~LicenseRecogRunParameter() noexcept {
}

LicenseRecogResult::LicenseRecogResult() {
}

LicenseRecogResult::~LicenseRecogResult() {
}

LicenseRecog::LicenseRecog() : impl_(nullptr) {
}

LicenseRecog::~LicenseRecog() noexcept {
  LicenseRecog::Release();
}

ErrorType LicenseRecog::Init(const InitParameter *p) {
  Release();
#ifdef  MXNET_ALLOWED
  impl_ = new arctern::LicenseRecogImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtLicenseRecogImpl;
  auto ret =arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasLicenseRecogImpl;
  auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType LicenseRecog::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void LicenseRecog::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
