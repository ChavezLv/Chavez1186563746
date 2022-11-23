/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.31
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceMask.h"

#ifdef MXNET_ALLOWED
#include "./faceMaskImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiFaceMaskImpl.h"
#elif defined(TVM_ALLOWED)
#include "./tvm/tvmFaceMaskImpl.h"
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "rknn/rknnFaceMaskImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/faceMask/trt/TrtFaceMaskImpl.h"
#elif defined(RKNN2_ALLOWED)
#include "src/Algorithms/Rknn2InterfaceCommonOperation.h"
#include "rknn2/rknn2FaceMaskImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasFaceMaskImpl.h"
#endif

using arctern::FaceMask;
using arctern::FaceMaskRunParameter;
using arctern::FaceMaskResult;
using arctern::ErrorType;
using arctern::Result;

FaceMaskRunParameter::FaceMaskRunParameter() {
}

FaceMaskRunParameter::~FaceMaskRunParameter()  {
}

FaceMaskResult::FaceMaskResult() {
}

FaceMaskResult::~FaceMaskResult() {
}

FaceMask::FaceMask() : impl_(nullptr) {
}

FaceMask::~FaceMask()  {
  FaceMask::Release();
}

ErrorType FaceMask::Init(const InitParameter *p) {
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceMaskImpl();
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiFaceMaskImpl;
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceMaskImpl();
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFaceMaskImpl;
  ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceMaskImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(RKNN2_ALLOWED)
  impl_ = new arctern::Rknn2FaceMaskImpl;
  ret = arctern::Rknn2InterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new arctern::AtlasFaceMaskImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx *)impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType FaceMask::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceMask::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}
