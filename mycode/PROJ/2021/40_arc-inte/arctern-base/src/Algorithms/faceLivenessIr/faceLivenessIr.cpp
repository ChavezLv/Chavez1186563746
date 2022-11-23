/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceLivenessIr.h"

#ifdef MXNET_ALLOWED
#include "./faceLivenessIrImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiFaceLivenessIrImpl.h"
#elif defined(TVM_ALLOWED)
#include "./tvm/tvmFaceLivenessIrImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "rknn/rknnFaceLivenessIrImpl.h"
#elif defined(RKNN2_ALLOWED)
#include "src/Algorithms/Rknn2InterfaceCommonOperation.h"
#include "rknn2/rknn2FaceLivenessIrImpl.h"
#endif

using arctern::FaceLivenessIr;
using arctern::FaceLivenessIrRunParameter;
using arctern::FaceLivenessIrResult;
using arctern::ErrorType;
using arctern::Result;

FaceLivenessIrRunParameter::FaceLivenessIrRunParameter() {
}

FaceLivenessIrRunParameter::~FaceLivenessIrRunParameter()  {
}

FaceLivenessIrResult::FaceLivenessIrResult() {
}

FaceLivenessIrResult::~FaceLivenessIrResult() {
}

FaceLivenessIr::FaceLivenessIr() : impl_(nullptr) {
}

FaceLivenessIr::~FaceLivenessIr()  {
}

ErrorType FaceLivenessIr::Init(const InitParameter *p) {
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceLivenessIrImpl();
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiFaceLivenessIrImpl;
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceLivenessIrImpl();
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFaceLivenessIrImpl;
  ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN2_ALLOWED)
  impl_ = new arctern::Rknn2FaceLivenessIrImpl;
  ret = arctern::Rknn2InterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#endif
  return ret;
}

ErrorType FaceLivenessIr::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceLivenessIr::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
